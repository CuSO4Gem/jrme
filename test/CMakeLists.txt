set(SRC_DIR ${PROJECT_SOURCE_DIR}/src/)

link_libraries(dl gtest m pthread)

set(JRME_SRC    ${SRC_DIR}date.cpp ${SRC_DIR}EnTimeParser.cpp)
add_executable(test_timeparser test_timeparser.cpp ${JRME_SRC})

set(JRME_SRC    ${SRC_DIR}Journal.cpp ${SRC_DIR}TxtJournalIO.cpp)
add_executable(test_TxtJournalIO test_TxtJournalIO.cpp ${JRME_SRC})

set(JRME_SRC    ${SRC_DIR}journal_release.c ${SRC_DIR}date.cpp ${SRC_DIR}Journal.cpp ${SRC_DIR}JournalIOFactory.cpp 
                ${SRC_DIR}JrmeConfig.cpp ${SRC_DIR}PluginJournalIO.cpp ${SRC_DIR}SfJournalBook.cpp 
                ${SRC_DIR}TxtJournalIO.cpp ${SRC_DIR}Utils.cpp)
add_executable(test_SfJournalBook test_SfJournalBook.cpp ${JRME_SRC})

set(JRME_SRC    ${SRC_DIR}journal_release.c ${SRC_DIR}date.cpp ${SRC_DIR}Journal.cpp ${SRC_DIR}JournalFilter.cpp
                ${SRC_DIR}JournalIOFactory.cpp ${SRC_DIR}JrmeConfig.cpp ${SRC_DIR}PluginJournalIO.cpp
                ${SRC_DIR}SfJournalBook.cpp ${SRC_DIR}TxtJournalIO.cpp ${SRC_DIR}Utils.cpp)
add_executable(test_JournalFilter test_JournalFilter.cpp ${JRME_SRC})

set(JRME_SRC    ${SRC_DIR}journal_release.c ${SRC_DIR}date.cpp ${SRC_DIR}Journal.cpp ${SRC_DIR}JournalIOFactory.cpp
                ${SRC_DIR}JrmeConfig.cpp ${SRC_DIR}PluginJournalIO.cpp ${SRC_DIR}SfJournalBook.cpp
                ${SRC_DIR}TxtJournalIO.cpp ${SRC_DIR}Utils.cpp)
add_executable(test_mdJournalIO test_mdJournalIO.cpp ${JRME_SRC})

set(JRME_SRC    ${SRC_DIR}journal_release.c ${SRC_DIR}date.cpp ${SRC_DIR}Journal.cpp ${SRC_DIR}JournalIOFactory.cpp
                ${SRC_DIR}JrmeConfig.cpp ${SRC_DIR}PluginJournalIO.cpp ${SRC_DIR}SfJournalBook.cpp
                ${SRC_DIR}TxtJournalIO.cpp ${SRC_DIR}Utils.cpp)
add_executable(test_JourrnalIOFactory test_JourrnalIOFactory.cpp ${JRME_SRC})

set(JRME_SRC    ${SRC_DIR}journal_release.c ${SRC_DIR}date.cpp  ${SRC_DIR}Journal.cpp ${SRC_DIR}ConfigNodeMaster.cpp ${SRC_DIR}JournalIOFactory.cpp
                ${SRC_DIR}JrmeConfig.cpp ${SRC_DIR}PluginJournalIO.cpp ${SRC_DIR}SfJournalBook.cpp
                ${SRC_DIR}TxtJournalIO.cpp ${SRC_DIR}Utils.cpp)
add_executable(test_ConfigNode test_ConfigNode.cpp ${JRME_SRC})

file(COPY ./testFile DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
file(COPY ./testAll.sh DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})