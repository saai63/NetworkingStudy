DOWNLOAD_FLAG="_download_complete_"
BUILD_FLAG="_build_complete_"

build_boost()
{
   # Check if build is already done
   if [ -e ${BUILD_FLAG} ]; then
      echo "Build is already done"
   else
      for item in `ls -d boost*`; do
         if [ -d $item ]; then
            BOOST_FOLDER=${item}
            echo "Deleting exisiting folder"
            rm -rf ${BOOST_FOLDER}
            break
         fi
      done
      echo "Extraction started . . ."
      tar -xvf ${BOOST_ARCHIVE} 2>/dev/null 1>/dev/null 
      echo "Extraction complete"
      for item in `ls -d boost*`; do
         if [ -d $item ]; then
            BOOST_FOLDER=${item}
            break
         fi
      done
      cd ${BOOST_FOLDER}
      ./bootstrap.sh
      ./b2
      if [ $? == 0 ]; then
         echo "Boost built successfully"
         cd ..
         touch ${BUILD_FLAG} 
      fi
   fi
}

if [ -e ${DOWNLOAD_FLAG} ]; then
   BOOST_ARCHIVE=`ls boost*.tar* 2>/dev/null`
   if [ "x"${BOOST_ARCHIVE} != "x" ];then
      echo "Found boost archive ${BOOST_ARCHIVE}"
      build_boost
   else
      echo "Boost archive is not available."
      echo "Please run get_boost.py to download boost"
   fi
else
   echo "Boost archive is not available."
   echo "Please run get_boost.py to download boost"
fi
