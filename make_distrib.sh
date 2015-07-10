#!/bin/bash
distribdir=../shallow
cp -vf *.h *.d *.i ${distribdir}
cp -vf Yworkdir/*.h Yworkdir/*.hpp Yworkdir/*.cpp  ${distribdir} 
if [ ! -f ${distribdir}/include ]
then
    mkdir ${distribdir}/include
fi
cp -vf RunModelM1QN3.hpp  ${distribdir}/include
cp -vf $YAODIR/yao/include/RunModelStd.hpp ${distribdir}/include
cp -vf $YAODIR/yao/include/RunModel.hpp ${distribdir}/include
cp -vf $YAODIR/yao/include/Observations.hpp ${distribdir}/include
cp -vf $YAODIR/yao/include/Module.hpp ${distribdir}/include
cp -vf $YAODIR/yao/include/Space.hpp ${distribdir}/include
cp -vf $YAODIR/yao/include/Dyniob.h ${distribdir}/include
cp -vf $YAODIR/yao/include/Project.hpp ${distribdir}/include
cp -vf $YAODIR/yao/include/Model.hpp ${distribdir}/include
cp -vf $YAODIR/yao/include/Trajectory.hpp ${distribdir}/include
cp -vf Makefile ${distribdir}