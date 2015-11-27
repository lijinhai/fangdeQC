#include "thread.h"
#include "mainWindow.h"

static int totalSize = 0;
Thread::Thread()
{
    stopped = false;
    stop = true;
}

void Thread::run()
{
    stopped = false;
    inputFlag = 1;
    stop = true;
    totalSize = 0;
    QStringList text = content.split("\n");
     for(int i = 0;i < text.length(); i++)
     {
         QFile cFile(text[i]);
         QString toPath;
         if(toDirecory[0] != "/")
             toPath = toDirecory[0] + "/" +  text[i].split("/")[text[i].split("/").length() - 1] ;
         else
             toPath = toDirecory[0] +  text[i].split("/")[text[i].split("/").length() - 1] ;

         if(!cFile.exists())
         {
             inputFlag = -2;
             stopped = true;
             return;
         }
        if(!copyFile(text[i],toPath))
        {
            inputFlag = -1;
            stopped = true;
            return;
        }
     }
    stopped = true;
}

void Thread::setInformation(QStringList tDirecory,QString cutString)
{
    content = cutString;
    toDirecory = tDirecory;
}


bool Thread::copyDirectoryFiles(const QString &fromDir,const QString &toDir)
{
        QDir sourceDir(fromDir);
        QDir targetDir(toDir);
        if(!targetDir.exists())
        {
               /**< 如果目标目录不存在，则进行创建 */
                if(!targetDir.mkdir(targetDir.absolutePath()))
                    return false;
        }
        QFileInfoList fileInfoList = sourceDir.entryInfoList();
        foreach(QFileInfo fileInfo, fileInfoList)
        {
            if(fileInfo.fileName() == "." || fileInfo.fileName() == "..")
                continue;
            if(fileInfo.isDir())
            {
                /**< 当为目录时，递归的进行copy */
                if(!copyDirectoryFiles(fileInfo.filePath(),
                    targetDir.filePath(fileInfo.fileName())))
                    return false;
            }
            else
            {
                /**< 当允许覆盖操作时，将旧文件进行删除操作 */
                if(targetDir.exists(fileInfo.fileName()))
                {
                    targetDir.remove(fileInfo.fileName());
                }

                /// 进行文件copy
                if(!copy(fileInfo.filePath(),targetDir.filePath(fileInfo.fileName())))
                {
                        return false;
                }
            }
        }
        return true;
}

bool Thread::copyFile(QString sPath,QString tPath)
{

    while(sPath == tPath)
    {
        tPath = changeFileName(tPath);
    }
   QFileInfo fileInfo(sPath);
   QFile cFile(tPath);
   if(cFile.exists())
   {
               cFile.remove();
   }

   if(fileInfo.isDir())
   {
        if(!copyDirectoryFiles(sPath,tPath))
        {
            return false;
        }
   }
   else
   {
        if(!copy(sPath,tPath))
        {
            return false;
        }
   }

   return true;
}


int Thread::copy(QString sPath, QString tPath)
{
        std::string sTemp = sPath.toStdString();
        const char *source = sTemp.c_str();
        std::string tTemp = tPath.toStdString();
        const char *target =tTemp.c_str();
        FILE * in,* out;
        int i = 0;
        int ch;
        if( (in = fopen(source,"rb"))  == NULL)
        {
            qDebug() << "Copy file error: Open file sPath failed!";
            return  0;
        }

        if((out = fopen(target,"wb")) == NULL)
        {
            qDebug() << "Copy file error: Open file tPath failed!";
            return  0;
        }
        while(!feof(in))
        {
                if(!stop)
                    break;
                ch = fgetc(in);
                /*10M file*/
                if(totalSize== 1048576* 10)
                {
                        mainWindow::dnum++;
                        totalSize = 0;
                 }

                  if(ch > -1)
                        fputc(ch,out);
                totalSize++;
        }
        fclose(in);
        fclose(out);
        return 1;
}

int Thread::getInputFlag()
{
        return inputFlag;
}

bool Thread:: getCopyFlag()
{
    return stopped;
}

void Thread ::overThreadStop()
{
     stop = false;
}

QString Thread::changeFileName(QString name)
{
    QFile cFile(name);
    QString baseName;
    QString suffixName;
    QString absolutePath;
    int i = 0;
    while(cFile.exists())
    {
        QFileInfo fileInfo(name);
        baseName = fileInfo.baseName();
        suffixName = fileInfo.completeSuffix();   // return name + fileSuffix;
        absolutePath = fileInfo.absolutePath();
        if(i == 0)
        {
                baseName += "(副本)";
        }
        else if(i == 1)
        {
            baseName = baseName.replace("副本" ,"副本" + QString::number(i));
        }
        else
        {
            baseName = baseName.replace("副本" + QString::number(i -1),"副本" + QString::number(i));
        }
        if(suffixName.isEmpty())
        {
                 name = absolutePath +"/" + baseName;
        }
        else
        {
            name = absolutePath +"/" + baseName + "." +suffixName;
        }
        i++; 
        cFile.setFileName(name);
    }
    return name ;
}
