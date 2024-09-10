# Define the JAR file
set(JAR_FILE odfvalidator-0.12.0-jar-with-dependencies.jar)

# Define the installation directory
set(JAR_INSTALL_DIR ${CMAKE_INSTALL_DATADIR}/java/odfvalidator)

# Install the JAR file
install(FILES ${JAR_FILE} DESTINATION ${JAR_INSTALL_DIR})
