/****************************************************************************
*   Copyright (C) 2012 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#include "exttool.h"
#include "messagedialog.h"
#include "settings.h"

#include <QFileInfo>
#include <QProcess>
#include <QString>
#include <QStringList>

//#if defined(_MSC_VER) && defined(_DEBUG)
//#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
//#define new DEBUG_NEW
//#endif // _MSC_VER

namespace {
   QStringList splitCommand(QString command)
   {
      QStringList args;
      QString tmp;
      int quoteCount = 0;
      bool inQuote = false;

      // handle quoting. tokens can be surrounded by double quotes
      // "hello world". three consecutive double quotes represent
      // the quote character itself.
      for (int i = 0; i < command.size(); ++i) {
         if (command.at(i) == u'"') {
            ++quoteCount;
            if (quoteCount == 3) {
               // third consecutive quote
               quoteCount = 0;
               tmp += command.at(i);
            }
            continue;
         }
         if (quoteCount) {
            if (quoteCount == 1)
               inQuote = !inQuote;
            quoteCount = 0;
         }
         if (!inQuote && command.at(i).isSpace()) {
            if (!tmp.isEmpty()) {
               args += tmp;
               tmp.clear();
            }
         }
         else {
            tmp += command.at(i);
         }
      }
      if (!tmp.isEmpty())
         args += tmp;

      return args;
   }
}

ExtTool::ExtTool(QObject *parent) : QObject(parent), m_extToolProcess(0)
{

}

ExtTool::~ExtTool()
{

}

void ExtTool::extToolReadOutput()
{
    while(m_extToolProcess && m_extToolProcess->canReadLine())
    {
        QString message = m_extToolProcess->readLine().simplified();
        errText.append(message);
        errText.append("\n");
    }
}

void ExtTool::RunExtTool1(QString pathIn)
{
    errText.clear();

    QString extTool1 = AppSettings->getValue("Tools/Path1").toString();
    QFileInfo fiExtTool(extTool1);
    QFileInfo fiPathIn(pathIn);
    m_extToolProcess = new QProcess(this);
    if(fiExtTool.exists() && fiPathIn.exists() && m_extToolProcess)
    {
        m_extToolProcess->setReadChannel(QProcess::StandardError);
        m_extToolProcess->setWorkingDirectory(fiPathIn.absolutePath());
        connect(m_extToolProcess, SIGNAL(readyReadStandardOutput()), SLOT(extToolReadOutput()));

        QString cmd = fiExtTool.absoluteFilePath();

        QString cmdLine = AppSettings->getValue("Tools/CommandLine1").toString();
        cmdLine.replace("$(InputPath)",fiPathIn.absoluteFilePath());
        cmdLine.replace("$(InputFile)",fiPathIn.fileName());
        cmdLine.replace("$(InputDir)",fiPathIn.absolutePath());
        cmdLine.replace("$(InputName)",fiPathIn.baseName());

        QStringList parameters = splitCommand(cmdLine);
        m_extToolProcess->start(cmd, parameters);

        if (!m_extToolProcess->waitForFinished())
        {
            MessageDialog::warning(fiExtTool.baseName() + ": " + m_extToolProcess->errorString());
        }
        else
        {
            extToolReadOutput();
            if (!errText.isEmpty())
            {
                MessageDialog::warning(fiExtTool.baseName() + ": " +errText);
            }
        }
    }
    else
    {
        MessageDialog::warning(fiExtTool.baseName() + tr(": File not found"));
    }
}
