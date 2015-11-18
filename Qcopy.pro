include(qxtglobalshortcut/qxtglobalshortcut.pri)
TRANSLATIONS = basicdraw_zh_CN.ts
LIBS += /usr/lib/x86_64-linux-gnu/libX11.so.6
QT += sql
QT +=network

unix {

SOURCES +=qtlockedfile/qtlockedfile_unix.cpp

}
SOURCES += \
    mainWindow.cpp \
    main.cpp \
    cutDialog.cpp \
    editDialog.cpp \
    attrDialog.cpp \
    helpDialog.cpp \
   qtsingleapplication/qtlocalpeer.cpp \
   qtsingleapplication/qtsingleapplication.cpp \
   qtsingleapplication/qtsinglecoreapplication.cpp \
   qtlockedfile/qtlockedfile.cpp \
    thread.cpp

HEADERS += \
    mainWindow.h \
    editDialog.h \
    cutDialog.h \
    attrDialog.h \
    helpDialog.h \
    qtsingleapplication/qtlocalpeer.h \
    qtsingleapplication/qtsingleapplication.h \
    qtsingleapplication/qtsinglecoreapplication.h \
    qtlockedfile/qtlockedfile.h \
     thread.h

RESOURCES += \
    image.qrc \
    Ch.qrc
