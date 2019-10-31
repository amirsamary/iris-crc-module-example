FROM intersystemsdc/irisdemo-base-irisdb-community:version-1.2

# Add and register the shared library. Must be done as root.
USER root
ADD ./libCRC.so /usr/crc/libCRC.so
RUN echo /usr/crc/ >> /etc/ld.so.conf.d/crc.conf && \
    ldconfig -v

# Going back to irisowner
USER irisowner

# Adding source code that will be loaded by the installer
ARG IRIS_PROJECT_FOLDER_NAME=test-project
ADD --chown=irisowner:irisuser ./${IRIS_PROJECT_FOLDER_NAME}/ $IRIS_APP_SOURCEDIR

# Loading the source code
RUN $ISC_PACKAGE_INSTALLDIR/demo/irisdemoinstaller.sh