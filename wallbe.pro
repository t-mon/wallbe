include(plugins.pri)

TARGET = $$qtLibraryTarget(guh_devicepluginwallbe)

message(============================================)
message(Qt version: $$[QT_VERSION])
message("Building $$deviceplugin$${TARGET}.so")

SOURCES += \
    devicepluginwallbe.cpp \

HEADERS += \
    devicepluginwallbe.h \
