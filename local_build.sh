#!/bin/bash

NORMAL="\\033[0;39m"
SUCCESS="\\033[1;32m"
FAILURE="\\033[1;31m"

export ARCH=x86
export ROOT_DIR=$(cd ../..; pwd)
export BUILD_DIR=`pwd`

export TOOLCHAIN_BASE_DIR=
export CROSS_COMPILE=

#export LD_LIBRARY_PATH=$TOOLCHAIN_BASE_DIR/lib


#Start: OF-DPA export for make-oflib
export OFDPA_BASE=`pwd`
export OF_DPA_LIB_DIR=$OFDPA_BASE/output/arm-orangePi-pc2/libraries
export OF_DPA_OUTPUT_DIR=$OFDPA_BASE/output/arm-orangePi-pc2
export OF_AGENT=indigo
export APP_BASE_DIR=$OFDPA_BASE/application/husky
export DELIVERABLES_DIR=$OFDPA_BASE/output/arm-orangePi-pc2
export OUTPATH=$DELIVERABLES_DIR

#End: OF-DPA export for make-oflib









#检查当前用户权限
#if [ ! $(id -u) -ne 0 ]; then
#    echo "Current user is in root mode , Start building ..."
#else
#    echo -e "\033[5m Please switch to root mode before compiling : sudo -s \033[0m"
#    exit 1
#fi



#if [ ! -d /opt/fsl-networking/QorIQ-SDK-V1.7 ]; then
#    rm -rf $ROOTFS_DIR
#    tar -zxvf $PRODUCT_RITP_DIR/toolchains/QorIQ-SDK-V1.7.tar.gz -C /opt/fsl-networking/
#fi






make_app()
{

    
    cd $APP_BASE_DIR
    
    make $1    
    
    if [ $? -eq 0 ];then 
        #chmod +x $ROOTFS_DIR/rsp/ritpapp
        echo -en ${SUCCESS}
        if [ "$1" != "clean" ];then 
            cp $OUTPATH/application/husky $BUILD_DIR/../orangepi
        fi
        echo "make app $1 success"
        echo -en ${NORMAL}
    else
        echo -en ${FAILURE}
        echo "make app $1 failed"
        echo -en ${NORMAL}
        exit 1
    fi


}

make_oflib()
{
    cd $OUTPATH
    echo "\n [Compiling oflib].........."
    
#    export -n CPATH
    
#    make ofagentapp
    make ofagent_libraries    
    if [ $? -eq 0 ];then 
        echo -en ${SUCCESS}
        echo "make ofagent_libraries success"
        echo -en ${NORMAL}
    else
        echo -en ${FAILURE}
        echo "make ofagent_libraries failed"
        echo -en ${NORMAL}
        exit 1
    fi

    make core
    if [ $? -eq 0 ];then 
        echo -en ${SUCCESS}
        echo "make oflib core success"
        echo -en ${NORMAL}
    else
        echo -en ${FAILURE}
        echo "make oflib core failed"
        echo -en ${NORMAL}
        exit 1
    fi

}


make_cli()
{
    cd $OUTPATH
    echo "\n [Compiling oflib].........."

    make cli
    if [ $? -eq 0 ];then 
        echo -en ${SUCCESS}
        echo "make oflib cli success"
        cp $OUTPATH/flowtable_dump $BUILD_DIR/../orangepi
        cp $OUTPATH/controller $BUILD_DIR/../orangepi
        cp -a $OUTPATH/librpc_client.so $BUILD_DIR/../orangepi
        cp $OUTPATH/librpc_client.so.1 $BUILD_DIR/../orangepi
        echo -en ${NORMAL}
        
    else
        echo -en ${FAILURE}
        echo "make oflib cli failed"
        echo -en ${NORMAL}
        exit 1
    fi

}


make_var_dump()
{
    cd $OUTPATH
    echo "[Compiling oflib].........."

    make dump-variables


}

clean_oflib()
{
    cd $OUTPATH
    echo "Cleaning oflib..."
    make clean-all
}


clean_all()
{
    rm -rf $OBJ_DIR/*
}



print_help()
{
    echo -e "Help:\n\tbuild.sh (all|app|oflib) [gdb] [clean]\n"
}


if [ $# -eq 0 ]
then
    print_help
else
    if [[ $# -eq 2 && "$2" == "gdb" ]]; then
        GDB_START=1
    fi
    
    case $1 in
     a|all)
        if [ "$2" == "clean" ]; then
            echo "clean all"
            clean_oflib
            make_app clean
        else
            echo "Make All"


            echo "make oflib..."
            make_oflib     
            
            
            echo "make app"
            make_app    


        fi
        ;;

    app)
        if [ "$2" == "clean" ]; then
            echo "clean app"
            make_app $2
            
        else
            echo "make oflib..."
            make_oflib 
            echo "make app..."
            make_app $2
        fi
        ;;
    oflib)
        if [ "$2" == "clean" ]; then
            echo "clean oflib"
            clean_oflib
        else
            echo "make oflib..."
            make_oflib 
        fi
        ;;
    cli)
        if [ "$2" == "clean" ]; then
            echo "clean oflib"
            clean_oflib
        else
            echo "make oflib..."
            make_cli 
        fi
        ;;
    debug)
        make_var_dump
        ;;
        *)
        print_help
        exit 1
        ;;

     esac
fi
