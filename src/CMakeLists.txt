include_directories (${CMAKE_SOURCE_DIR}/src
                     ${TASK_INCLUDE_DIRS})

set (task_SRCS API.cpp API.h Att.cpp Att.h Cmd.cpp Cmd.h Color.cpp Color.h
               Config.cpp Config.h Context.cpp Context.h Date.cpp Date.h
               Directory.cpp Directory.h Duration.cpp Duration.h File.cpp
               File.h Filter.cpp Filter.h Grid.cpp Grid.h Hooks.cpp Hooks.h
               JSON.cpp JSON.h Keymap.cpp Keymap.h Lisp.cpp Lisp.h Location.cpp
               Location.h Nibbler.cpp Nibbler.h Path.cpp Path.h Permission.cpp
               Permission.h Record.cpp Record.h Rectangle.cpp Rectangle.h
               Sensor.cpp Sensor.h Sequence.cpp Sequence.h StringTable.cpp
               StringTable.h Subst.cpp Subst.h TDB.cpp TDB.h Table.cpp Table.h
               Task.cpp Task.h Taskmod.cpp Taskmod.h Thread.cpp Thread.h
               Timer.cpp Timer.h Transport.cpp Transport.h TransportSSH.cpp
               TransportSSH.h TransportRSYNC.cpp TransportRSYNC.h
               TransportCurl.cpp TransportCurl.h Tree.cpp Tree.h burndown.cpp
               command.cpp custom.cpp dependency.cpp diag.cpp edit.cpp
               export.cpp history.cpp i18n.h import.cpp interactive.cpp
               recur.cpp report.cpp rules.cpp rx.cpp rx.h text.cpp text.h
               utf8.cpp utf8.h util.cpp util.h Uri.cpp Uri.h)

add_library (task STATIC ${task_SRCS})
add_executable (task_executable main.cpp)
target_link_libraries (task_executable task ${TASK_LIBRARIES})
set_property (TARGET task_executable PROPERTY OUTPUT_NAME "task")

install (TARGETS task_executable DESTINATION ${TASK_BINDIR})

set (CMAKE_BUILD_TYPE debug)
set (CMAKE_C_FLAGS_DEBUG "-ggdb3")
set (CMAKE_C_FLAGS_RELEASE "-O3")
