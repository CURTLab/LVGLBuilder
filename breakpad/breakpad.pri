INCLUDEPATH += $$PWD

win32{
HEADERS += $$PWD/common/windows/guid_string.h \
	$$PWD/client/windows/handler/exception_handler.h \
	$$PWD/client/windows/crash_generation/crash_generation_server.h \
	$$PWD/client/windows/crash_generation/crash_generation_client.h

SOURCES += $$PWD/common/windows/guid_string.cc \
	$$PWD/common/windows/string_utils.cc \
        $$PWD/client/windows/handler/exception_handler.cc \
	$$PWD/client/windows/crash_generation/crash_generation_client.cc
}

win64{
HEADERS += $$PWD/common/windows/guid_string.h \
        $$PWD/client/windows/handler/exception_handler.h \
        $$PWD/client/windows/crash_generation/crash_generation_server.h \
        $$PWD/client/windows/crash_generation/crash_generation_client.h

SOURCES += $$PWD/common/windows/guid_string.cc \
        $$PWD/common/windows/string_utils.cc \
        $$PWD/client/windows/handler/exception_handler.cc \
        $$PWD/client/windows/crash_generation/crash_generation_client.cc
}


unix{
HEADERS += $$PWD/common/linux/guid_creator.h \
        $$PWD/client/linux/handler/exception_handler.h \
        $$PWD/client/linux/crash_generation/crash_generation_server.h \
        $$PWD/client/linux/crash_generation/crash_generation_client.h

SOURCES += $$PWD/common/linux/guid_creator.cc \
        $$PWD/client/linux/handler/exception_handler.cc \
        $$PWD/client/linux/crash_generation/crash_generation_client.cc
}
