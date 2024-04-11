TEMPLATE = subdirs
 
SUBDIRS += \
    src/Lib \
    src/App \

# явно указываем pro-файлы этих проектов
src/lib.file = src/Lib/Lib.pro
src/app.file = src/App/App.pro

src/App.depends = src/Lib
