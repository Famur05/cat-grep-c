#!/bin/bash


SUCCESS=0
FAIL=0
COUNTER=0
DIFF_RES=""

# Classic
declare -a tests=(
"-s VAR s test_grep/first.txt"
"-s VAR s test_grep/second.txt"
)

# several files
declare -a tests1=(
"-s VAR s test_grep/first.txt test_grep/second.txt"
"-s VAR s test_grep/second.txt test_grep/first.txt"
"-s VAR \"[0-9]\" test_grep/first.txt test_grep/second.txt"
)

# Haven't search pattern
declare -a tests2=(
"-s VAR s test_grep/for.txt"
"-s VAR s test_grep/first.txt test_grep/for.txt"
"-s VAR s no_file.txt"
)

# Specific
declare -a extra=(
"-s s test_grep/first.txt"
"-s s test_grep/second.txt"
"-s s test_grep/first.txt test_grep/second.txt"
"-s s test_grep/second.txt test_grep/first.txt"
"-s \"[0-9]\" test_grep/first.txt test_grep/second.txt"
)

testing()
{
    t=$(echo $@ | sed "s/VAR/$var/")
    ./s21_grep $t > test_s21_grep.log
    grep $t > test_sys_grep.log
    DIFF_RES="$(diff -s test_s21_grep.log test_sys_grep.log)"
    (( COUNTER++ ))
    if [ "$DIFF_RES" == "Files test_s21_grep.log and test_sys_grep.log are identical" ]
    then
      (( SUCCESS++ ))
      echo "$FAIL/$SUCCESS - $COUNTER  SUCCESS  grep $t"
    else
      (( FAIL++ ))
      echo "$FAIL/$SUCCESS - $COUNTER  FAIL  grep $t"
    fi
    rm test_s21_grep.log test_sys_grep.log
}


# 1 параметр
for var1 in e i v c l n h o
do
    for i in "${tests[@]}"
    do
        var="-$var1"
        testing $i
    done
done

# 1 параметр & 2 file
for var1 in e i v c l n h o
do
    for i in "${tests1[@]}"
    do
        var="-$var1"
        testing $i
    done
done

# 1 параметр & not search pattern
for var1 in e i v c l n h o
do
    for i in "${tests2[@]}"
    do
        var="-$var1"
        testing $i
    done
done

# специфические тесты
for i in "${extra[@]}"
do
    var="-"
    testing $i
done


# 2 параметра
for var1 in e i v c l n h o
do
    for var2 in e i v c l n h o
    do
        if [ $var1 != $var2 ]
        then
            for i in "${tests[@]}"
            do
                var="-$var1 -$var2"
                testing $i
            done
        fi
    done
done

# 2 параметра & 2 file
for var1 in e i v c l n h o
do
    for var2 in e i v c l n h o
    do
        if [ $var1 != $var2 ]
        then
            for i in "${tests1[@]}"
            do
                var="-$var1 -$var2"
                testing $i
            done
        fi
    done
done

# 2 параметра & not search pattern
for var1 in e i v c l n h o
do
    for var2 in e i v c l n h o
    do
        if [ $var1 != $var2 ]
        then
            for i in "${tests2[@]}"
            do
                var="-$var1 -$var2"
                testing $i
            done
        fi
    done
done

# 3 параметра
for var1 in e i v c l n h o
do
    for var2 in e i v c l n h o
    do
        for var3 in e i v c l n h o
        do
            if [ $var1 != $var2 ] && [ $var2 != $var3 ] && [ $var1 != $var3 ]
            then
                for i in "${tests[@]}"
                do
                    var="-$var1 -$var2 -$var3"
                    testing $i
                done
            fi
        done
    done
done

# 3 параметра & 2 file
for var1 in e i v c l n h o
do
    for var2 in e i v c l n h o
    do
        for var3 in e i v c l n h o
        do
            if [ $var1 != $var2 ] && [ $var2 != $var3 ] && [ $var1 != $var3 ]
            then
                for i in "${tests1[@]}"
                do
                    var="-$var1 -$var2 -$var3"
                    testing $i
                done
            fi
        done
    done
done

# 3 параметра & not search pattern
for var1 in e i v c l n h o
do
    for var2 in e i v c l n h o
    do
        for var3 in e i v c l n h o
        do
            if [ $var1 != $var2 ] && [ $var2 != $var3 ] && [ $var1 != $var3 ]
            then
                for i in "${tests2[@]}"
                do
                    var="-$var1 -$var2 -$var3"
                    testing $i
                done
            fi
        done
    done
done

# 2 сдвоенных параметра
for var1 in e i v c l n h o
do
    for var2 in e i v c l n h o
    do
        if [ $var1 != $var2 ]
        then
            for i in "${tests[@]}"
            do
                var="-$var1$var2"
                testing $i
            done
        fi
    done
done

# 2 сдвоенных параметра & 2 file
for var1 in e i v c l n h o
do
    for var2 in e i v c l n h o
    do
        if [ $var1 != $var2 ]
        then
            for i in "${tests1[@]}"
            do
                var="-$var1$var2"
                testing $i
            done
        fi
    done
done

# 2 сдвоенных параметра & not search pattern
for var1 in e i v c l n h o
do
    for var2 in e i v c l n h o
    do
        if [ $var1 != $var2 ]
        then
            for i in "${tests2[@]}"
            do
                var="-$var1$var2"
                testing $i
            done
        fi
    done
done

# 3 строенных параметра
for var1 in e i v c l n h o
do
    for var2 in e i v c l n h o
    do
        for var3 in e i v c l n h o
        do
            if [ $var1 != $var2 ] && [ $var2 != $var3 ] && [ $var1 != $var3 ]
            then
                for i in "${tests[@]}"
                do
                    var="-$var1$var2$var3"
                    testing $i
                done
            fi
        done
    done
done

# 3 строенных параметра & file
for var1 in e i v c l n h o
do
    for var2 in e i v c l n h o
    do
        for var3 in e i v c l n h o
        do
            if [ $var1 != $var2 ] && [ $var2 != $var3 ] && [ $var1 != $var3 ]
            then
                for i in "${tests1[@]}"
                do
                    var="-$var1$var2$var3"
                    testing $i
                done
            fi
        done
    done
done

# 3 строенных параметра & not search pattern
for var1 in e i v c l n h o
do
    for var2 in e i v c l n h o
    do
        for var3 in e i v c l n h o
        do
            if [ $var1 != $var2 ] && [ $var2 != $var3 ] && [ $var1 != $var3 ]
            then
                for i in "${tests2[@]}"
                do
                    var="-$var1$var2$var3"
                    testing $i
                done
            fi
        done
    done
done

echo "FAIL: $FAIL"
echo "SUCCESS: $SUCCESS"
echo "ALL: $COUNTER"
