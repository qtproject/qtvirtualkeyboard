OTHER_FILES += \
            $$PWD/qtvkb.qdocconf \
            $$PWD/src/index.qdoc \
            $$PWD/src/intro.qdoc \
            $$PWD/src/examples/CustomInputMethod.qml

docs_target.target = docs
docs_target.commands = qdoc $$PWD/qtvkb.qdocconf

QMAKE_EXTRA_TARGETS = docs_target