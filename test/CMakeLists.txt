set(SRC_DIR ${PROJECT_SOURCE_DIR}/source/)

link_libraries(dl m pthread)

set(JRME_SRC    ${SRC_DIR}date.cpp ${SRC_DIR}EnTimeParser.cpp)
add_executable(test_timeparser test_timeparser.cpp ${JRME_SRC})
target_link_libraries(test_timeparser gtest)

set(JRME_SRC    ${SRC_DIR}Journal.cpp ${SRC_DIR}TxtJournalIO.cpp)
add_executable(test_TxtJournalIO test_TxtJournalIO.cpp ${JRME_SRC})
target_link_libraries(test_TxtJournalIO gtest)

set(JRME_SRC    ${SRC_DIR}journal_release.c ${SRC_DIR}date.cpp ${SRC_DIR}Journal.cpp ${SRC_DIR}JournalIOFactory.cpp 
                ${SRC_DIR}JrmeConfig.cpp ${SRC_DIR}PluginJournalIO.cpp ${SRC_DIR}SfJournalBook.cpp 
                ${SRC_DIR}TxtJournalIO.cpp ${SRC_DIR}Utils.cpp)
add_executable(test_SfJournalBook test_SfJournalBook.cpp ${JRME_SRC})
target_link_libraries(test_SfJournalBook gtest)

set(JRME_SRC    ${SRC_DIR}journal_release.c ${SRC_DIR}date.cpp ${SRC_DIR}Journal.cpp ${SRC_DIR}JournalFilter.cpp
                ${SRC_DIR}JournalIOFactory.cpp ${SRC_DIR}JrmeConfig.cpp ${SRC_DIR}PluginJournalIO.cpp
                ${SRC_DIR}SfJournalBook.cpp ${SRC_DIR}TxtJournalIO.cpp ${SRC_DIR}Utils.cpp)
add_executable(test_JournalFilter test_JournalFilter.cpp ${JRME_SRC})
target_link_libraries(test_JournalFilter gtest)

set(JRME_SRC    ${SRC_DIR}journal_release.c ${SRC_DIR}date.cpp ${SRC_DIR}Journal.cpp ${SRC_DIR}JournalIOFactory.cpp
                ${SRC_DIR}JrmeConfig.cpp ${SRC_DIR}PluginJournalIO.cpp ${SRC_DIR}SfJournalBook.cpp
                ${SRC_DIR}TxtJournalIO.cpp ${SRC_DIR}Utils.cpp)
add_executable(test_mdJournalIO test_mdJournalIO.cpp ${JRME_SRC})
target_link_libraries(test_mdJournalIO gtest)

set(JRME_SRC    ${SRC_DIR}journal_release.c ${SRC_DIR}date.cpp ${SRC_DIR}Journal.cpp ${SRC_DIR}JournalIOFactory.cpp
                ${SRC_DIR}JrmeConfig.cpp ${SRC_DIR}PluginJournalIO.cpp ${SRC_DIR}SfJournalBook.cpp
                ${SRC_DIR}TxtJournalIO.cpp ${SRC_DIR}Utils.cpp)
add_executable(test_JourrnalIOFactory test_JourrnalIOFactory.cpp ${JRME_SRC})
target_link_libraries(test_JourrnalIOFactory gtest)

set(JRME_SRC    ${SRC_DIR}journal_release.c ${SRC_DIR}date.cpp  ${SRC_DIR}Journal.cpp ${SRC_DIR}JAttributeMaster.cpp ${SRC_DIR}JournalIOFactory.cpp
                ${SRC_DIR}JrmeConfig.cpp ${SRC_DIR}PluginJournalIO.cpp ${SRC_DIR}SfJournalBook.cpp
                ${SRC_DIR}TxtJournalIO.cpp ${SRC_DIR}Utils.cpp)
add_executable(test_JAttribute test_JAttribute.cpp ${JRME_SRC})
target_link_libraries(test_JAttribute gtest)

file(COPY ./testFile DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
file(COPY ./testAll.sh DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})