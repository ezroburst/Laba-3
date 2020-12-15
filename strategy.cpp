#include "strategy.h"
void recMethod(QDir directory, double& size)//запись
{
    QFileInfoList filesInfo = directory.entryInfoList(QDir::NoDotAndDotDot | QDir::Files);//предоставляет независимую от системы информацию о файле
    QString dirName = directory.dirName();

    if (filesInfo.size())
    {
        for (auto i : filesInfo)
        {
            size += i.size();
        }
    }

    QStringList childs = directory.entryList(QDir::NoDotAndDotDot | QDir::AllDirs);
    for (auto& c : childs)
    {
        QDir dir = QDir(directory.absolutePath() + "\\" + c);
        recMethod(dir, size);
    }
}
//размер вложенных папок проц
double listFolder ( QString path,double size )
{
    QDir currentFolder( path );

    double totalsize = 0;

    currentFolder.setFilter( QDir::AllEntries);
    currentFolder.setSorting( QDir::Name );



    QFileInfoList folderitems( currentFolder.entryInfoList() );

    foreach ( QFileInfo i, folderitems )
    {
        QString iname( i.fileName() );
        if ( iname == "." || iname == ".." || iname.isEmpty() )
            continue;

        if ( i.isDir() )
            totalsize += listFolder( path+"/"+iname,size );
        else
            totalsize += i.size();//-----------------------------

    }
//totalsize=totalsize/1024;//в кб
   // totalsize=totalsize/size*100;
     //totalsize=totalsize/Allsize * 100;
    return totalsize;
}

//размер текущей папки проц
double sizeFolder(QString path,double size )
{
    QDir currentFolder( path );

    double totalsize = 0;

    currentFolder.setFilter( QDir::AllEntries);
    currentFolder.setSorting( QDir::Name );

    QFileInfoList folderitems( currentFolder.entryInfoList() );




    foreach ( QFileInfo i, folderitems )
    {
        //recMethod(currentFolder,size);
        QString iname( i.fileName() );
        if ( iname == "." || iname == ".." || iname.isEmpty() )
            continue;
        totalsize += i.size();//------------------------

    }

      //  totalsize=totalsize/Allsize * 100;
//totalsize=totalsize/size*100;//проц
  //  totalsize=totalsize/1024;//кб
    return totalsize;
}

//рамер файлов по их типам проц
Element sizeType (QString path, Element tmp,double size)
{

    QDir currentFolder( path );
    currentFolder.setFilter( QDir::AllEntries);
    currentFolder.setSorting( QDir::Name );

    QFileInfoList folderitems( currentFolder.entryInfoList() );

    foreach ( QFileInfo i, folderitems )
    {
        QString iname( i.fileName() );
        if ( iname == "." || iname == ".." || iname.isEmpty() )
            continue;
        if(i.isDir())
        {
            tmp = sizeType(path+"/"+iname, tmp,size);
        }
        if(i.isFile())
        {
            if(tmp.elementName.empty())
            {
                tmp.elementName<<i.completeSuffix();
               // tmp.elementSize<<i.size()/1024;
                tmp.elementSize<<i.size();
            //    tmp.elementPro<<i.size();

            }
            else
            {
                bool chek = false;
                foreach (QString str, tmp.elementName)
                {
                    if(i.completeSuffix() == str)
                    {
                        tmp.elementSize[tmp.elementName.indexOf(str,0)] += i.size();
                        chek=true;
                    }
                }
                if(!chek)
                {
                    tmp.elementName<<i.completeSuffix();
                  // tmp.elementSize<<i.size()/1024;//в кб
                     tmp.elementSize<<i.size();
                   //  tmp.elementPro<<i.size();
                }
            }
        }
    }


//    for(int i=0; i<tmp.elementSize.size(); i++)
//    {
//    tmp.elementSize[i]=tmp.elementSize[i]/size*100;
//    }

    return tmp;
}


//стретегия по папкам
void folderSrtategy::UseStrategy(const QModelIndex &index, QFileSystemModel *model, Element& element)
{
    element.elementName.clear();
    element.elementSize.clear();
    element.elementPro.clear();
    double size=0;

    QFileInfo fileinfo = model->fileInfo(index);
    QDir currentFolder( fileinfo.filePath() );
    currentFolder.setFilter( QDir::Dirs);
    currentFolder.setSorting( QDir::Name );
    QFileInfoList folderitems( currentFolder.entryInfoList() );
    foreach ( QFileInfo i, folderitems )
    {
    if ( i.isDir() )
    {
        recMethod(currentFolder,size);//вот это число на которое делить size!!!!!!
        size;
        break;
    }
    }
    foreach ( QFileInfo i, folderitems )
    {
        QString iname( i.fileName() );
        if ( iname == "." || iname == ".." || iname.isEmpty() )
            continue;

        if ( i.isDir() )
        {


            element.elementName<<iname;
            element.elementSize<<listFolder(fileinfo.filePath()+"/"+iname,size);
           // element.elementPro<<i.size();
        }
        else
            continue;

    }



    //размер файлов текущей папки
    element.elementName<<"current folder";
    element.elementSize<<sizeFolder(fileinfo.filePath(),size);
    foreach ( QFileInfo i, folderitems )
    {
        element.elementPro<<i.size();
    }
    for(int i=0; i< element.elementSize.size(); i++)//
    {

     element.elementPro[i]= element.elementSize[i]/1024;
    }
    for(int i=0; i< element.elementSize.size(); i++)//проценты
    {

     element.elementSize[i]= element.elementSize[i]/size*100;
    }


}

//стратегия по типам файлов
void fileTypeStrategy::UseStrategy(const QModelIndex &index, QFileSystemModel *model, Element& element)
{

    element.elementName.clear();
    element.elementSize.clear();
     element.elementPro.clear();
    QFileInfo fileinfo = model->fileInfo(index);
    QDir currentFolder( fileinfo.filePath() );
    currentFolder.setFilter( QDir::AllEntries);
    currentFolder.setSorting( QDir::Name );
    QFileInfoList folderitems( currentFolder.entryInfoList() );
bool flag=false;
int k=0;
    double size=0;
    foreach ( QFileInfo i, folderitems )
    {
    if ( i.isDir() )
    {
        recMethod(currentFolder,size);//вот это число на которое делить size!!!!!!
        size;
        break;
    }
    }


    foreach ( QFileInfo i, folderitems )
    {

        QString iname( i.fileName() );
        if ( iname == "." || iname == ".." || iname.isEmpty() )
            continue;
        if(i.isFile())
        {
            if(element.elementName.empty())//true если пуст
            {
                element.elementName<<i.completeSuffix();
                element.elementSize<<i.size();
//element.elementPro<<i.size();


            }
            else
            {
                bool chek = false;
                foreach (QString str, element.elementName)
                {
                    if(i.completeSuffix() == str)
                    {
                        element.elementSize[element.elementName.indexOf(str,0)] += i.size();
                        chek=true;
                    }



                }
                if(!chek)
                {
                    element.elementName<<i.completeSuffix();
                    element.elementSize<<i.size();
                   // element.elementPro<<i.size();
                }
            }
        }


        if(i.isDir())
        {
            element = sizeType(fileinfo.filePath()+"/"+iname, element,size);

        }
    }
    foreach ( QFileInfo i, folderitems )
    {
        element.elementPro<<i.size();
    }
    for(int i=0; i< element.elementSize.size(); i++)//
    {

     element.elementPro[i]= element.elementSize[i]/1024;
    }
            for(int i=0; i< element.elementSize.size(); i++)//проценты
            {

             element.elementSize[i]= element.elementSize[i]/size*100;
            }




}
