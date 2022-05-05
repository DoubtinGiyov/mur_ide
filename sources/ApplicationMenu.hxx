#pragma once

#include "QmlUtils.hxx"

#include <QObject>

namespace Ide::Ui {

class ApplicationMenu : public QObject {
    Q_OBJECT
    Q_PROPERTY(QStringList examples READ getExamples CONSTANT)

public:
  static ApplicationMenu *instance;
  static void Create();
  QStringList getExamples();
  QString GetPathToDirExamples();

  public slots:
      void onFileNew();
      void onFileOpen();
      void onFileSave();
      void onFileSaveAs();
      void onFindFind();

      void onCodeRun();
      void onCodeStop();
      void onRunSimulator();
      void onRunRemote();

      void onTargetModeChanged();

      void onViewIncreaseFontSize();
      void onViewDecreaseFontSize();
      void onViewResetFontSize();
      void onViewToggleEditor();

      void onHelpDocumentation();
      void onHelpAbout();
      void onHelpPreferences();
      void onHelpVisitOnGitHub();
      void onHelpSendFeedback();
      void onHelpExample(const QString &);

  private:
      QStringList m_examples;
      static qml::RegisterType<ApplicationMenu> Register;
      void init();
      ApplicationMenu();
      QString m_pathToDirExamples;
};
}
