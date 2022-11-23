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
#include "asm.h"
#include "../Constants.h"

namespace  QSynedit {

const QSet<QString> ASMHighlighter::Keywords {
    "aaa","aad","aam","adc","add","and","arpl","bound","bsf","bsr","bswap","bt","btc","btr","bts",
    "call","cbw","cdq","clc","cld","cli","clts","cmc","cmp","cmps","cmpsb","cmpsd","cmpsw",
    "cmpxchg","cwd","cwde","daa","das","dec","div","emms","enter","f2xm1","fabs","fadd","faddp","fbld",
    "fbstp","fchs","fclex","fcmovb","fcmovbe","fcmove","fcmovnb","fcmovnbe","fcmovne","fcmovnu",
    "fcmovu","fcom","fcomi","fcomip","fcomp","fcompp","fcos","fdecstp","fdiv","fdivp","fdivr",
    "fdivrp","femms","ffree","fiadd","ficom","ficomp","fidiv","fidivr","fild","fimul","fincstp",
    "finit","fist","fistp","fisub","fisubr","fld","fld1","fldcw","fldenv","fldl2e","fldl2t","fldlg2",
    "fldln2","fldpi","fldz","fmul","fmulp","fnclex","fninit","fnop","fnsave","fnstcw","fnstenv",
    "fnstsw","fpatan","fprem1","fptan","frndint","frstor","fsave","fscale","fsin","fsincos",
    "fsqrt","fst","fstcw","fstenv","fstp","fstsw","fsub","fsubp","fsubr","fsubrp","ftst",
    "fucom","fucomi","fucomip","fucomp","fucompp","fwait","fxch","fxtract","fyl2xp1","hlt","idiv",
    "imul","in","inc","ins","insb","insd","insw","int","into","invd","invlpg","iret","iretd","iretw",
    "ja","jae","jb","jbe","jc","jcxz","je","jecxz","jg","jge","jl","jle","jmp","jna","jnae","jnb","jnbe","jnc",
    "jne","jng","jnge","jnl","jnle","jno","jnp","jns","jnz","jo","jp","jpe","jpo","js","jz","lahf","lar","lds",
    "lea","leave","les","lfs","lgdt","lgs","lidt","lldt","lmsw","lock","lods","lodsb","lodsd","lodsw",
    "loop","loope","loopne","loopnz","loopz","lsl","lss","ltr","mov","movd","movq"," movs","movsb",
    "movsd","movsw","movsx","movzx","mul","neg","nop","not","or","out","outs","outsb","outsd","outsw",
    "packssdw","packsswb","packuswb","paddb","paddd","paddsb","paddsw","paddusb","paddusw",
    "paddw","pand","pandn","pavgusb","pcmpeqb","pcmpeqd","pcmpeqw","pcmpgtb","pcmpgtd","pcmpgtw",
    "pf2id","pfacc","pfadd","pfcmpeq","pfcmpge","pfcmpgt","pfmax","pfmin","pfmul","pfrcp",
    "pfrcpit1","pfrcpit2","pfrsqit1","pfrsqrt","pfsub","pfsubr","pi2fd","pmaddwd","pmulhrw",
    "pmulhw","pmullw","pop","popa","popad","popaw","popf","popfd","popfw","por","prefetch","prefetchw",
    "pslld","psllq","psllw","psrad","psraw","psrld","psrlq","psrlw","psubb","psubd","psubsb",
    "psubsw","psubusb","psubusw","psubw","punpckhbw","punpckhdq","punpckhwd","punpcklbw",
    "punpckldq","punpcklwd","push","pusha","pushad","pushaw","pushf","pushfd","pushfw","pxor",
    "rcl","rcr","rep","repe","repne","repnz","repz","ret","rol","ror","sahf","sal","sar","sbb","scas",
    "scasb","scasd","scasw","seta","setae","setb","setbe","setc","sete","setg","setge","setl","setle",
    "setna","setnae","setnb","setnbe","setnc","setne","setng","setnge","setnl","setnle","setno",
    "setnp","setns","setnz","seto","setp","setpo","sets","setz","sgdt","shl","shld","shr","shrd","sidt",
    "sldt","smsw","stc","std","sti","stos","stosb","stosd","stosw","str","sub","test","verr","verw",
    "wait","wbinvd","xadd","xchg","xlat","xlatb","xor"
};



ASMHighlighter::ASMHighlighter()
{
    mCommentAttribute = std::make_shared<HighlighterAttribute>(SYNS_AttrComment, TokenType::Comment);
    mCommentAttribute->setStyles(FontStyle::fsItalic);
    addAttribute(mCommentAttribute);
    mIdentifierAttribute = std::make_shared<HighlighterAttribute>(SYNS_AttrIdentifier, TokenType::Identifier);
    addAttribute(mIdentifierAttribute);
    mKeywordAttribute = std::make_shared<HighlighterAttribute>(SYNS_AttrReservedWord, TokenType::Keyword);
    mKeywordAttribute->setStyles(FontStyle::fsBold);
    addAttribute(mKeywordAttribute);
    mNumberAttribute = std::make_shared<HighlighterAttribute>(SYNS_AttrNumber, TokenType::Number);
    addAttribute(mNumberAttribute);
    mWhitespaceAttribute = std::make_shared<HighlighterAttribute>(SYNS_AttrSpace, TokenType::Space);
    addAttribute(mWhitespaceAttribute);
    mStringAttribute = std::make_shared<HighlighterAttribute>(SYNS_AttrString, TokenType::String);
    addAttribute(mStringAttribute);
    mSymbolAttribute = std::make_shared<HighlighterAttribute>(SYNS_AttrSymbol, TokenType::Operator);
    addAttribute(mSymbolAttribute);
}

PHighlighterAttribute ASMHighlighter::numberAttribute()
{
    return mNumberAttribute;
}

void ASMHighlighter::CommentProc()
{
    mTokenID = TokenId::Comment;
    do {
        mRun++;
    } while (! (mLine[mRun]==0 || mLine[mRun] == '\r' || mLine[mRun]=='\n'));
}

void ASMHighlighter::CRProc()
{
    mTokenID = TokenId::Space;
    mRun++;
    if (mLine[mRun] == '\n')
        mRun++;
}

void ASMHighlighter::GreaterProc()
{
    mRun++;
    mTokenID = TokenId::Symbol;
    if (mLine[mRun] == '=')
        mRun++;
}

void ASMHighlighter::IdentProc()
{
    int start = mRun;
    while (isIdentChar(mLine[mRun])) {
        mRun++;
    }
    QString s = mLineString.mid(start,mRun-start);
    if (Keywords.contains(s)) {
        mTokenID = TokenId::Key;
    } else {
        mTokenID = TokenId::Identifier;
    }
}

void ASMHighlighter::LFProc()
{
    mTokenID = TokenId::Space;
    mRun++;
}

void ASMHighlighter::LowerProc()
{
    mRun++;
    mTokenID = TokenId::Symbol;
    if (mLine[mRun]=='=' || mLine[mRun]== '>')
        mRun++;
}

void ASMHighlighter::NullProc()
{
    mTokenID = TokenId::Null;
}

void ASMHighlighter::NumberProc()
{
    mRun++;
    mTokenID = TokenId::Number;
    while (true) {
        QChar ch = mLine[mRun];
        if (!((ch>='0' && ch<='9') || (ch=='.') || (ch >= 'a' && ch<='f')
              || (ch=='h') || (ch >= 'A' && ch<='F') || (ch == 'H')))
            break;
        mRun++;
    }
}

void ASMHighlighter::SingleQuoteStringProc()
{
    mTokenID = TokenId::String;
    if ((mRun+2 < mLineString.size()) && (mLine[mRun + 1] == '\'') && (mLine[mRun + 2] == '\''))
        mRun += 2;
    while (true) {
        if (mLine[mRun] == 0 || mLine[mRun] == '\r' || mLine[mRun] == '\n' || mLine[mRun] == '\'')
            break;
        mRun++;
    }
    if (mLine[mRun]!=0)
        mRun++;
}

void ASMHighlighter::SlashProc()
{
    mRun++;
    if (mLine[mRun] == '/') {
      mTokenID = TokenId::Comment;
      while (true) {
        mRun++;
        if (mLine[mRun] == 0 || mLine[mRun] == '\r' || mLine[mRun] == '\n')
            break;
      }
    } else
        mTokenID = TokenId::Symbol;
}

void ASMHighlighter::SpaceProc()
{
    mTokenID = TokenId::Space;
    while (true) {
        mRun++;
        if (mLine[mRun] == 0 || mLine[mRun] == '\r' || mLine[mRun] == '\n')
            break;
        if (mLine[mRun] > 32)
            break;
    }
}

void ASMHighlighter::StringProc()
{
    mTokenID = TokenId::String;
    if ((mRun+2 < mLineString.size()) && (mLine[mRun + 1] == '\"') && (mLine[mRun + 2] == '\"'))
        mRun += 2;
    while (true) {
        if (mLine[mRun] == 0 || mLine[mRun] == '\r' || mLine[mRun] == '\n')
            break;
        if (mLine[mRun] == '\"')
            break;
        mRun += 1;
    }
    if (mLine[mRun]!=0)
        mRun++;
}

void ASMHighlighter::SymbolProc()
{
    mRun++;
    mTokenID = TokenId::Symbol;
}

void ASMHighlighter::UnknownProc()
{
    mRun++;
    mTokenID = TokenId::Unknown;
}

bool ASMHighlighter::eol() const
{
    return mTokenID == TokenId::Null;
}

QString ASMHighlighter::languageName()
{
    return "asm";
}

HighlighterLanguage ASMHighlighter::language()
{
    return HighlighterLanguage::Asssembly;
}

QString ASMHighlighter::getToken() const
{
    return mLineString.mid(mTokenPos,mRun-mTokenPos);
}

PHighlighterAttribute ASMHighlighter::getTokenAttribute() const
{
    switch(mTokenID) {
    case TokenId::Comment:
        return mCommentAttribute;
    case TokenId::Identifier:
        return mIdentifierAttribute;
    case TokenId::Key:
        return mKeywordAttribute;
    case TokenId::Number:
        return mNumberAttribute;
    case TokenId::Space:
        return mWhitespaceAttribute;
    case TokenId::String:
        return mStringAttribute;
    case TokenId::Symbol:
        return mSymbolAttribute;
    case TokenId::Unknown:
        return mIdentifierAttribute;
    default:
        break;
    }
    return PHighlighterAttribute();
}

int ASMHighlighter::getTokenPos()
{
    return mTokenPos;
}

void ASMHighlighter::next()
{
    mTokenPos = mRun;
    switch(mLine[mRun].unicode()) {
    case 0:
        NullProc();
        break;
    case '\n':
        LFProc();
        break;
    case '\r':
        CRProc();
        break;
    case '\"':
        StringProc();
        break;
    case '\'':
        SingleQuoteStringProc();
        break;
    case '>':
        GreaterProc();
        break;
    case '<':
        LowerProc();
        break;
    case '/':
        SlashProc();
        break;
    case '#':
    case ';':
        CommentProc();
        break;
    case '.':
    case ':':
    case '&':
    case '{':
    case '}':
    case '=':
    case '^':
    case '-':
    case '+':
    case '(':
    case ')':
    case '*':
        SymbolProc();
        break;
    default:
        if (mLine[mRun]>='0' && mLine[mRun]<='9') {
            NumberProc();
        } else if ((mLine[mRun]>='A' && mLine[mRun]<='Z')
                   || (mLine[mRun]>='a' && mLine[mRun]<='z')
                   || (mLine[mRun]=='_')) {
            IdentProc();
        } else if (mLine[mRun]<=32) {
            SpaceProc();
        } else {
            UnknownProc();
        }
    }
}

void ASMHighlighter::setLine(const QString &newLine, int lineNumber)
{
    mLineString = newLine;
    mLine = mLineString.data();
    mLineNumber = lineNumber;
    mRun = 0;
    next();
}

HighlighterClass ASMHighlighter::getClass() const
{
    return HighlighterClass::CppHighlighter;
}

QString ASMHighlighter::getName() const
{
    return SYN_HIGHLIGHTER_CPP;
}

bool ASMHighlighter::getTokenFinished() const
{
    return true;
}

bool ASMHighlighter::isLastLineCommentNotFinished(int /*state*/) const
{
    return true;
}

bool ASMHighlighter::isLastLineStringNotFinished(int /*state*/) const
{
    return true;
}

HighlighterState ASMHighlighter::getState() const
{
    return HighlighterState();
}

void ASMHighlighter::setState(const HighlighterState&)
{

}

void ASMHighlighter::resetState()
{

}

QSet<QString> ASMHighlighter::keywords() const
{
    return Keywords;
}
}
