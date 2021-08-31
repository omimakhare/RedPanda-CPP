#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "common.h"
#include "widgets/searchresultview.h"
#include "widgets/classbrowser.h"
#include "widgets/codecompletionpopup.h"
#include "widgets/headercompletionpopup.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


enum class CompileTarget {
    Invalid, None, File, Project, SyntaxCheck
};

class EditorList;
class QLabel;
class QComboBox;
class CompilerManager;
class Editor;
class Debugger;
class CPUDialog;
class QPlainTextEdit;
class SearchDialog;

class MainWindow : public QMainWindow
{
    Q_OBJECT

    enum class CompileSuccessionTaskType {
        None,
        Run,
        Debug,
        Profile
    };

    struct CompileSuccessionTask {
        CompileSuccessionTaskType type;
        QString filename;
    };

    using PCompileSuccessionTask = std::shared_ptr<CompileSuccessionTask>;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void updateForEncodingInfo();
    void updateStatusbarForLineCol();
    void updateForStatusbarModeInfo();
    void updateStatusbarMessage(const QString& s);
    void updateEditorSettings();
    void updateEditorActions();
    void updateCompileActions();
    void updateEditorColorSchemes();
    void updateCompilerSet();
    void updateDebuggerSettings();
    void checkSyntaxInBack(Editor* e);
    bool compile(bool rebuild=false);
    void runExecutable(const QString& exeName, const QString& filename=QString());
    void runExecutable();
    void debug();
    void showSearchPanel();

    void applySettings();

    void removeActiveBreakpoints();
    void setActiveBreakpoint(QString FileName, int Line, bool setFocus=true);
    void updateAppTitle();
    void addDebugOutput(const QString& text);
    void changeDebugOutputLastline(const QString& text);
    void updateDebugEval(const QString& value);

    void rebuildOpenedFileHisotryMenu();

    void updateClassBrowserForEditor(Editor* editor);

    void resetAutoSaveTimer();

    QPlainTextEdit* txtLocals();

    CPUDialog *cpuDialog() const;

    Debugger *debugger() const;

    EditorList *editorList() const;

    SearchDialog *searchDialog() const;

    SearchResultModel* searchResultModel();

    const std::shared_ptr<CodeCompletionPopup> &completionPopup() const;

    const std::shared_ptr<HeaderCompletionPopup> &headerCompletionPopup() const;
public slots:
    void onCompileLog(const QString& msg);
    void onCompileIssue(PCompileIssue issue);
    void onCompileFinished();
    void onCompileErrorOccured(const QString& reason);
    void onRunErrorOccured(const QString& reason);
    void onRunFinished();
    void cleanUpCPUDialog();
    void onDebugCommandInput(const QString& command);
    void onDebugEvaluateInput();
    void onParserProgress(const QString& fileName, int total, int current);
    void onStartParsing();
    void onEndParsing(int total, int updateView);
    void onEvalValueReady(const QString& value);

protected:
    void openFiles(const QStringList& files);
    void openFile(const QString& filename);

private:
    CompileTarget getCompileTarget();
    bool debugInferiorhasBreakpoint();
    void setupActions();
    void openCloseMessageSheet(bool open);
    void prepareDebugger();
    void doAutoSave(Editor *e);
    void buildContextMenus();

private slots:
    void onAutoSaveTimeout();

    void on_actionNew_triggered();

    void on_EditorTabsLeft_tabCloseRequested(int index);

    void on_actionOpen_triggered();

    void on_actionSave_triggered();

    void on_actionSaveAs_triggered();

    void on_actionOptions_triggered();

    // qt will auto bind slots with the prefix "on_"
    void onCompilerSetChanged(int index);

    void on_actionCompile_triggered();

    void on_actionRun_triggered();

    void on_actionUndo_triggered();

    void on_actionRedo_triggered();

    void on_actionCut_triggered();

    void on_actionSelectAll_triggered();

    void on_actionCopy_triggered();

    void on_actionPaste_triggered();

    void on_actionIndent_triggered();

    void on_actionUnIndent_triggered();

    void on_actionToggleComment_triggered();

    void on_actionUnfoldAll_triggered();

    void on_actionFoldAll_triggered();

    void on_tableIssues_doubleClicked(const QModelIndex &index);

    void on_actionEncode_in_ANSI_triggered();

    void on_actionEncode_in_UTF_8_triggered();

    void on_actionAuto_Detect_triggered();

    void on_actionConvert_to_ANSI_triggered();

    void on_actionConvert_to_UTF_8_triggered();

    void on_tabMessages_tabBarClicked(int index);

    void on_tabMessages_currentChanged(int index);

    void on_tabMessages_tabBarDoubleClicked(int index);

    void on_actionCompile_Run_triggered();

    void on_actionRebuild_triggered();

    void on_actionStop_Execution_triggered();

    void on_actionDebug_triggered();

    void on_actionStep_Over_triggered();

    void on_actionStep_Into_triggered();

    void on_actionStep_Out_triggered();

    void on_actionRun_To_Cursor_triggered();

    void on_actionContinue_triggered();

    void on_actionAdd_Watch_triggered();

    void on_actionView_CPU_Window_triggered();

    void on_actionExit_triggered();

    void on_actionFind_triggered();

    void on_actionFind_in_files_triggered();

    void on_actionReplace_triggered();

    void on_actionFind_Next_triggered();

    void on_actionFind_Previous_triggered();

    void on_cbSearchHistory_currentIndexChanged(int index);

    void on_btnSearchAgin_clicked();
    void on_actionRemove_Watch_triggered();

    void on_actionRemove_All_Watches_triggered();

    void on_actionModify_Watch_triggered();

private:
    Ui::MainWindow *ui;
    EditorList *mEditorList;
    QLabel *mFileInfoStatus;
    QLabel *mFileEncodingStatus;
    QLabel *mFileModeStatus;
    QMenu *mMenuEncoding;
    QMenu *mMenuEncodingList;
    QMenu *mMenuRecentFiles;
    QComboBox *mCompilerSet;
    CompilerManager *mCompilerManager;
    Debugger *mDebugger;
    CPUDialog *mCPUDialog;
    SearchDialog *mSearchDialog;
    QList<QAction *> mRecentFileActions;
    bool mQuitting;
    QElapsedTimer mParserTimer;

    std::shared_ptr<CodeCompletionPopup> mCompletionPopup;
    std::shared_ptr<HeaderCompletionPopup> mHeaderCompletionPopup;

    SearchResultModel mSearchResultModel;
    PSearchResultListModel mSearchResultListModel;
    PSearchResultTreeModel mSearchResultTreeModel;
    PSearchResultTreeViewDelegate mSearchViewDelegate;
    ClassBrowserModel mClassBrowserModel;

    bool mMessageControlChanged;
    bool mTabMessagesTogglingState;
    bool mCheckSyntaxInBack;
    int mPreviousHeight;
    PCompileSuccessionTask mCompileSuccessionTask;

    QTimer mAutoSaveTimer;


   // QWidget interface
protected:
    void closeEvent(QCloseEvent *event) override;
};

extern MainWindow* pMainWindow;
#endif // MAINWINDOW_H
