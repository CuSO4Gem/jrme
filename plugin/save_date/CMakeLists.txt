set(PLUGIN_NAME save_date)

set(JRME_SRC_DIR ${PROJECT_SOURCE_DIR}/src/)

# 需要用的jrme的源码，但这不是插件必须的
# save_data need source code of jrme，but it is not necessary
set(JRME_SRC ${JRME_SRC_DIR}/date.cpp)

add_library(${PLUGIN_NAME} SHARED NodeUtil.cpp SaveDate.cpp ${JRME_SRC})