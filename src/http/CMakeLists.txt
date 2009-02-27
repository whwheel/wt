IF(CONNECTOR_HTTP)
  MESSAGE("** Enabling built-in httpd.")

  SET(libhttpsources
    Configuration.C
    Connection.C
    ConnectionManager.C
    HTTPRequest.C
    HTTPStream.C
    MimeTypes.C
    Reply.C
    Request.C
    RequestHandler.C
    RequestParser.C
    Server.C
    SslConnection.C
    StaticReply.C
    StockReply.C
    TcpConnection.C
    WServer.C
    WtReply.C
  )
    
  OPTION(HTTP_WITH_SSL "Support for SSL (https)" ${SSL_FOUND})
  OPTION(HTTP_WITH_ZLIB "Support for zlib (http compression)" ${ZLIB_FOUND})

  IF (HTTP_WITH_SSL)
    IF (NOT SSL_FOUND)
      MESSAGE(FATAL "HTTP_WITH_SSL enabled but could not find OpenSSL")
    ENDIF (NOT SSL_FOUND)
    ADD_DEFINITIONS(-DHTTP_WITH_SSL)
    SET(MY_SSL_LIBS ${SSL_LIBRARIES})
    INCLUDE_DIRECTORIES(${SSL_INCLUDE_DIRS})
  ELSE (HTTP_WITH_SSL)
    SET(MY_SSL_LIBS "")
  ENDIF (HTTP_WITH_SSL)

  IF(NOT BOOST_WTHTTP_FOUND)
    MESSAGE(FATAL "Could not find all boost libraries required to build the httpe connector (thread, fs, po, dt)")
  ENDIF(NOT BOOST_WTHTTP_FOUND)
  IF(NOT ASIO_FOUND)
    MESSAGE("** Error finding Wthttpd required library: ")
    MESSAGE(FATAL_ERROR "** Wthttpd requires the asio library (http://asio.sf.net), the boost or non-boost version 0.3.9 or later.")
  ENDIF(NOT ASIO_FOUND)
  IF(HTTP_WITH_ZLIB)
    ADD_DEFINITIONS(-DWTHTTP_WITH_ZLIB ${ZLIB_DEFINITIONS})
    SET(MY_ZLIB_LIBS ${ZLIB_LIBRARIES})
    INCLUDE_DIRECTORIES(${ZLIB_INCLUDE_DIRS})
  ELSE(HTTP_WITH_ZLIB)
    SET(MY_ZLIB_LIBS "")
  ENDIF(HTTP_WITH_ZLIB)

  INCLUDE_DIRECTORIES(
    ${ASIO_INCLUDE_DIRS}
    ${BOOST_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/../web
    ${CMAKE_CURRENT_SOURCE_DIR}/../wt
    ${CMAKE_CURRENT_BINARY_DIR}/
  )

  ADD_DEFINITIONS(${ZLIB_DEFINITIONS} ${ASIO_DEFINITIONS})
  ADD_LIBRARY(wthttp ${libhttpsources})

  TARGET_LINK_LIBRARIES(wthttp
    wt
    ${MY_ZLIB_LIBS}
    ${MY_SSL_LIBS}
    ${BOOST_WTHTTP_LIBRARIES}
    ${ASIO_LIBRARIES}
  )

  INSTALL(TARGETS wthttp
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION ${LIB_INSTALL_DIR}
    ARCHIVE DESTINATION ${LIB_INSTALL_DIR})

  SET_TARGET_PROPERTIES(
    wthttp
  PROPERTIES
    VERSION ${VERSION_SERIES}.${VERSION_MAJOR}.${VERSION_MINOR}
    SOVERSION ${WTHTTP_SOVERSION}
    DEBUG_POSTFIX "d"
  )

  IF(WIN32)
    SET_TARGET_PROPERTIES(
        wthttp
      PROPERTIES
        COMPILE_FLAGS "/wd4251 /wd4275 /wd4355"
    )
  ENDIF(WIN32)

  ADD_DEPENDENCIES(wthttp wt ${EXAMPLES_CONNECTOR})

ELSE(CONNECTOR_HTTP)
  MESSAGE("** Disabling built-in httpd.")
ENDIF(CONNECTOR_HTTP)
