/*
 * Copyright (C) 2020-2022 Roy Qu (royqh1979@gmail.com)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#include "sdccfilecompiler.h"
#include "utils.h"
#include "compilermanager.h"
#include "../systemconsts.h"

#include <QFile>
#include <QFileInfo>
#include <QMessageBox>


SDCCFileCompiler::SDCCFileCompiler(const QString &filename, const QByteArray &encoding,
                           CppCompileType compileType, bool onlyCheckSyntax):
    Compiler(filename, onlyCheckSyntax),
    mEncoding(encoding),
    mCompileType(compileType),
    mNoStartup(false)
{

}

bool SDCCFileCompiler::prepareForCompile()
{
    //compilerSet()->setCompilationStage(Settings::CompilerSet::CompilationStage::GenerateExecutable);

    if (mOnlyCheckSyntax) {
        mCompiler = compilerSet()->CCompiler();
        mArguments += getCCompileArguments(false);
        mArguments += getCIncludeArguments();
        mArguments += getProjectIncludeArguments();
        mArguments = QString("--syntax-only \"%1\"").arg(mFilename);
        mDirectory = extractFileDir(mFilename);
        return true;
    }
    log(tr("Compiling single file..."));
    log("------------------");
    log(tr("- Filename: %1").arg(mFilename));
    log(tr("- Compiler Set Name: %1").arg(compilerSet()->name()));
    log("");

    QString strFileType = "C";
    mCompiler = compilerSet()->CCompiler();
    mArguments += getCCompileArguments(false);
    mArguments += getCIncludeArguments();
    mArguments += getProjectIncludeArguments();

    if (!fileExists(mCompiler)) {
        throw CompileError(
                    tr("The Compiler '%1' doesn't exists!").arg(mCompiler)
                    +"<br />"
                    +tr("Please check the \"program\" page of compiler settings."));
    }

    mOutputFile=changeFileExt(mFilename, compilerSet()->executableSuffix());
    mIhxFilename = changeFileExt(mFilename,SDCC_IHX_SUFFIX);

    QString val = compilerSet()->compileOptions().value(SDCC_OPT_NOSTARTUP);
    mNoStartup = (val==COMPILER_OPTION_ON);
    if (mNoStartup) {
        mRelFilename = changeFileExt(mFilename,SDCC_REL_SUFFIX);
        mArguments += QString(" -c \"%1\"").arg(mFilename);
        mExtraCompilersList.append(mCompiler);
        QString args = getLibraryArguments(FileType::CSource);
        args += QString(" -o \"%1\" \"%2\" ").arg(mIhxFilename, mRelFilename);
        mExtraArgumentsList.append(args);
        mExtraOutputFilesList.append("");
    } else {
        mArguments += getLibraryArguments(FileType::CSource);
        mArguments += QString(" \"%1\"").arg(mFilename);
        mArguments+=QString(" -o \"%1\"").arg(mIhxFilename);
    }

    if (compilerSet()->executableSuffix() == SDCC_HEX_SUFFIX) {
        QString packihx = compilerSet()->findProgramInBinDirs(PACKIHX_PROGRAM);
        if (packihx.isEmpty()) {
            error(tr("Can't find \"%1\".\n").arg(PACKIHX_PROGRAM));
            return false;
        }
        mExtraCompilersList.append(packihx);
        QString args;
        args = QString(" \"%1\"").arg(mIhxFilename);
        mExtraArgumentsList.append(args);
        mExtraOutputFilesList.append(mOutputFile);
    } else if (compilerSet()->executableSuffix() == SDCC_BIN_SUFFIX) {
        QString makebin = compilerSet()->findProgramInBinDirs(MAKEBIN_PROGRAM);
        if (makebin.isEmpty()) {
            error(tr("Can't find \"%1\".\n").arg(PACKIHX_PROGRAM));
            return false;
        }
        mExtraCompilersList.push_back(makebin);
        QString args;
        args = QString(" \"%1\"").arg(mIhxFilename);
        args+=QString(" \"%1\"").arg(mOutputFile);
        mExtraArgumentsList.push_back(args);
        mExtraOutputFilesList.append("");
    }

    log(tr("Processing %1 source file:").arg(strFileType));
    log("------------------");
    log(tr("- %1 Compiler: %2").arg(strFileType).arg(mCompiler));
    log(tr("- Command: %1 %2").arg(extractFileName(mCompiler)).arg(mArguments));
    mDirectory = extractFileDir(mFilename);
    mStartCompileTime = QDateTime::currentDateTime();
    return true;
}

bool SDCCFileCompiler::beforeRunExtraCommand(int idx)
{
    if (mNoStartup) {
        if (idx==0) {
            QFileInfo file(mRelFilename);
            return file.exists() && (file.lastModified()>mStartCompileTime);
        } else if (idx==1) {
            QFileInfo file(mIhxFilename);
            return file.exists() && (file.lastModified()>mStartCompileTime);
        }
    } else {
        if (idx==0) {
            QFileInfo file(mIhxFilename);
            return file.exists() && (file.lastModified()>mStartCompileTime);
        }
    }
    return true;
}

bool SDCCFileCompiler::prepareForRebuild()
{
    QString exeName=compilerSet()->getOutputFilename(mFilename);

    QFile file(exeName);

    if (file.exists() && !file.remove()) {
        QFileInfo info(exeName);
        throw CompileError(tr("Can't delete the old executable file \"%1\".\n").arg(info.absoluteFilePath()));
    }
    return true;
}
