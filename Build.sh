#!/bin/bash

NAME=LDAbs
LIBNAME=lib$NAME

mkdir BUILD
mkdir BUILD/tmp

mkdir BUILD/include
mkdir BUILD/include/$NAME
mkdir BUILD/GCC
mkdir BUILD/GCC/x86-32
mkdir BUILD/GCC/x86-64
mkdir BUILD/MINGW
mkdir BUILD/MINGW/x86-32
mkdir BUILD/MINGW/x86-64

INCLUDEFOLDER=BUILD/include/$NAME


cp *.h $INCLUDEFOLDER
cp -r ThreadPool/ $INCLUDEFOLDER
cp -r LinkedList/ $INCLUDEFOLDER


Compile (){
cd BUILD/tmp

$1 -c $2 ../../ThreadPool/$6.c -o ThreadPool.o
$1 -c $2 ../../LinkedList/GeneralLinkedList.c 
$1 -c $2 ../../Weak_Stack_Queue.c ../../Object_Pool.c ../../SinglyLinkedList.c ../../DoublyLinkedList.c
$3 rcs $4.a GeneralLinkedList.o  Weak_Stack_Queue.o Object_Pool.o SinglyLinkedList.o DoublyLinkedList.o ThreadPool.o
   
$1 -c $2 -fPIC ../../ThreadPool/$6.c -o ThreadPool.o
$1 -c $2 -fPIC ../../LinkedList/GeneralLinkedList.c 
$1 -c $2 -fPIC ../../Weak_Stack_Queue.c ../../Object_Pool.c ../../SinglyLinkedList.c ../../DoublyLinkedList.c
$1 $2 -O3 -shared -o $4.$8 GeneralLinkedList.o  Weak_Stack_Queue.o Object_Pool.o SinglyLinkedList.o DoublyLinkedList.o ThreadPool.o $7


mv $4* $5

cd ../..
}


echo "GCC 64"
Compile gcc -m64 ar $LIBNAME ../GCC/x86-64/ PThreadpool -lpthread so

echo "GCC 32"
Compile gcc -m32 ar $LIBNAME ../GCC/x86-32/ PThreadpool -lpthread so

echo "MINGW 32"
Compile x86_64-w64-mingw32-gcc -m32  x86_64-w64-mingw32-ar $LIBNAME ../MINGW/x86-32/ PThreadpool -lpthread dll

echo "MINGW 64"
Compile x86_64-w64-mingw32-gcc -m64 x86_64-w64-mingw32-ar $LIBNAME ../MINGW/x86-64/ PThreadpool -lpthread dll

