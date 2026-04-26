cmake_minimum_required(VERSION 3.10)
project(ASCIIPaint VERSION 1.0.0 LANGUAGES CXX) 

# Устанавливаем стандарт C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Для Windows - включаем консольный режим
if(WIN32)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -mconsole")
endif()

# Устанавливаем директории для заголовочных файлов
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)

# Находим исходные файлы
file(GLOB SOURCES "${SRC_DIR}/*.cpp")

# Создаем исполняемый файл
add_executable(${PROJECT_NAME} ${SOURCES})

# Добавляем директории для поиска заголовков
target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIR})

# Подключаем необходимые библиотеки для Windows
if(WIN32)
    target_link_libraries(${PROJECT_NAME} 
        user32
        gdi32
    )
endif()

# Настройки для разных типов сборки
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")

# Выводим информацию о конфигурации
message(STATUS "Project: ${PROJECT_NAME}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Source files: ${SOURCES}")