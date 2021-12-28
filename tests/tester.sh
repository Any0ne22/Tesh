#!/bin/bash
# Made by Tom Gouville & Omar Chida
set -e

echo '######### Installing dependencies'
[[ -z $INDOCKER ]] || apt-get update
[[ -z $INDOCKER ]] || apt-get -y install --no-install-recommends build-essential libreadline-dev electric-fence python3 python3-pip
pip install ptyprocess


echo '######### Trying to build'
rm -f tesh # in case it was committed
make || (echo ERROR: make failed; exit 1)
if [ ! -f tesh ]; then
  echo ERROR 'make' did not generate a 'tesh' file
  exit 1
fi

if ldd tesh | grep -q libasan; then
  echo "ERROR 'tesh' is linked with libasan. Did you build with -fsanitize?"
  exit 1
fi

ti=`mktemp /tmp/output.XXXXXX`
to=`mktemp /tmp/output.XXXXXX`
te=`mktemp /tmp/output.XXXXXX`
currentdir=$(pwd)
Red='\033[0;31m'          # Red
Green='\033[0;32m'        # Green
Color_Off='\033[0m'       # Text Reset
test_nb=0
test_passed=0
test_failed=0

display() {
echo Input:
echo ------------------------------------------------
cat $ti
echo ------------------------------------------------
echo Output:
echo ------------------------------------------------
cat $to
echo ------------------------------------------------
echo Expected output:
echo ------------------------------------------------
cat $te
echo ------------------------------------------------
mo=`md5sum < $to | awk '{print $1}'`
me=`md5sum < $te | awk '{print $1}'`
test_nb=$((test_nb+1))
if ! [ "$mo" = "$me" ]; then
  test_failed=$((test_failed+1))
  printf "${Red}==> Test#${test_nb} : ERROR outputs do not match.${Color_Off}\n"
  #exit 1
else
  test_passed=$((test_passed+1))
  printf "${Green}==> Test#${test_nb} : passed successfully.${Color_Off}\n"
fi
echo -e "\n\n"
}

displayNotTest() {
echo Input:
echo ------------------------------------------------
cat $ti
echo ------------------------------------------------
echo Output:
echo ------------------------------------------------
cat $to
echo ------------------------------------------------
echo Expected output:
echo ------------------------------------------------
cat $te
echo ------------------------------------------------
echo -e "\n\n"
}

# enable Electric Fence
export LD_PRELOAD=libefence.so.0.0
export EF_DISABLE_BANNER=1
export EF_ALLOW_MALLOC_0=1


echo '######### Trying to test tesh with a basic script (two commands)'
cat <<-EOF > $ti 2>&1
echo foo
echo bar baz
EOF
cat <<-EOF > $te 2>&1
foo
bar baz
EOF
timeout 2s bash -c "./tesh < $ti" > $to 2>&1
display $ti $to $te

echo '######### Trying to test tesh with a more advanced script (commands chaining)'
cat <<-EOF > $ti 2>&1
false ; echo A
true ; echo B
false && echo a
true && echo b
false || echo c
true || echo d
EOF
cat <<-EOF > $te 2>&1
A
B
b
c
EOF
timeout 2s bash -c "./tesh < $ti" > $to 2>&1
display $ti $to $te

echo '######### Trying to test tesh with a more advanced script (input/output redirections)'
cat <<-EOF > $ti 2>&1
echo foo > /tmp/toto
echo bar > /tmp/toto
echo baz >> /tmp/toto
cat < /tmp/toto
EOF
cat <<-EOF > $te 2>&1
bar
baz
EOF
timeout 2s bash -c "./tesh < $ti" > $to 2>&1
display $ti $to $te


echo '######### Trying to test tesh with many commands'
cat <<-EOF > $ti 2>&1
echo foo bar baz
echo bar
echo 1
echo 2
echo 3
echo 4
echo 5
echo 6
EOF
cat <<-EOF > $te 2>&1
foo bar baz
bar
1
2
3
4
5
6
EOF
timeout 2s bash -c "./tesh < $ti" > $to 2>&1
display $ti $to $te

echo '######### Trying to test tesh with andor2'
cat <<-EOF > $ti 2>&1
false && echo a || echo b || echo c && echo d || echo e && echo f && false || echo g ; false && echo a || echo b || echo c && echo d || echo e && echo f && false || echo g
EOF
cat <<-EOF > $te 2>&1
b
d
f
g
b
d
f
g
EOF
timeout 2s bash -c "./tesh < $ti" > $to 2>&1
display $ti $to $te

echo '######### Trying to test tesh built in cd'
cat <<-EOF > $ti 2>&1
cd /
pwd
EOF
cat <<-EOF > $te 2>&1
/
EOF
timeout 2s bash -c "./tesh < $ti" > $to 2>&1
display $ti $to $te

echo '######### Trying to test redirections'
cat <<-EOF > $ti 2>&1
cat tests/lorem-ipsum > /tmp/TMPFILE
echo sortie de echo
cat /tmp/TMPFILE
EOF
cat <<-EOF > $te 2>&1
sortie de echo
Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.
EOF
timeout 2s bash -c "./tesh < $ti" > $to 2>&1
display $ti $to $te

echo '######### Trying to test redirections'
cat <<-EOF > $ti 2>&1
cat tests/lorem-ipsum > /tmp/TMPFILE
echo sortie de echo
cat /tmp/TMPFILE
EOF
cat <<-EOF > $te 2>&1
sortie de echo
Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.
EOF
timeout 2s bash -c "./tesh < $ti" > $to 2>&1
display $ti $to $te


echo '######### Trying to test redirections'
cat <<-EOF > $ti 2>&1
cat tests/lorem-ipsum > /tmp/TMPFILE
echo sortie de echo
cat /tmp/TMPFILE
EOF
cat <<-EOF > $te 2>&1
sortie de echo
Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.
EOF
timeout 2s bash -c "./tesh < $ti" > $to 2>&1
display $ti $to $te


echo '######### Trying to test redirections 2'
cat <<-EOF > $ti 2>&1
cat < tests/lorem-ipsum
EOF
cat <<-EOF > $te 2>&1
Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.
EOF
timeout 2s bash -c "./tesh < $ti" > $to 2>&1
display $ti $to $te


echo '######### Trying to test redirections 3'
cat <<-EOF > $ti 2>&1
cat tests/lorem-ipsum > /tmp/TMPFILE
cat tests/lorem-ipsum > /tmp/TMPFILE
cat tests/lorem-ipsum >> /tmp/TMPFILE
echo sortie de echo
cat /tmp/TMPFILE
EOF
cat <<-EOF > $te 2>&1
sortie de echo
Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.
Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.
EOF
timeout 2s bash -c "./tesh < $ti" > $to 2>&1
display $ti $to $te


echo '######### Trying to test pipe 1'
cat <<-EOF > $ti 2>&1
cat tests/lorem-ipsum-long | grep Duis
EOF
cat <<-EOF > $te 2>&1
Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur.
EOF
timeout 2s bash -c "./tesh < $ti" > $to 2>&1
display $ti $to $te


echo '######### Trying to test pipe 2'
cat <<-EOF > $ti 2>&1
cat tests/lorem-ipsum-long | grep Duis
EOF
cat <<-EOF > $te 2>&1
Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur.
EOF
timeout 2s bash -c "./tesh < $ti" > $to 2>&1
display $ti $to $te


echo '######### Trying to test pipe 2'
cat <<-EOF > $ti 2>&1
cat tests/lorem-ipsum-long | grep am | grep Ut > /tmp/TMPFILE ; cat tests/lorem-ipsum-long | grep Lorem >> /tmp/TMPFILE ; tac < /tmp/TMPFILE
EOF
cat <<-EOF > $te 2>&1
Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.
Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat.
EOF
timeout 2s bash -c "./tesh < $ti" > $to 2>&1
display $ti $to $te

echo '######### Trying to test -e'
cat <<-EOF > $ti 2>&1
echo a
echo b
false
echo c
EOF
cat <<-EOF > $te 2>&1
a
b
EOF
timeout 2s bash -c "./tesh -e < $ti" > $to 2>&1
display $ti $to $te

 
echo '######### Trying to test prompt'
cat <<-EOF > $ti 2>&1
echo foo
EOF
cat <<-EOF > $te 2>&1
`whoami`@`hostname`:`pwd`$ echo foo
foo
`whoami`@`hostname`:`pwd`$ 
EOF
timeout 2s bash -c "python3 tests/ptytester.py ./tesh 'echo foo'" > $to 2>&1
display $ti $to $te


echo '######### Trying to test prompt 2'
cat <<-EOF > $ti 2>&1
cd /
EOF
cat <<-EOF > $te 2>&1
`whoami`@`hostname`:`pwd`$ cd /
`whoami`@`hostname`:/$ 
EOF
timeout 2s bash -c "python3 tests/ptytester.py ./tesh 'cd /'" > $to 2>&1
display $ti $to $te

if [[ -z $INDOCKER ]]; then

echo '######### Trying to test fd leaks'
cat <<-EOF > $ti 2>&1
./tests/fdtester.sh cat ./tests/lorem-ipsum > /tmp/TMPFILE
echo sortie de echo
cat /tmp/TMPFILE
EOF
cat <<-EOF > $te 2>&1
sortie de echo
0 -> ***
1 -> ***
2 -> ***
Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.
EOF
timeout 2s bash -c "./tesh < $ti" > $to 2>&1
displayNotTest $ti $to $te
test_nb=$((test_nb+1))
if ! grep -Pzo "sortie de echo\n0 ->.*\n1 ->.*\n2 ->.*\nLorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua." $to > /dev/null; then
  echo "(Ne pas tester dans VSCode)"
  test_failed=$((test_failed+1))
  printf "${Red}==> Test#${test_nb} : ERROR outputs do not match.${Color_Off}\n"
else 
  test_passed=$((test_passed+1))
  printf "${Green}==> Test#${test_nb} : passed successfully.${Color_Off}\n"
fi


echo '######### Trying to test fd leaks 2'
cat <<-EOF > $ti 2>&1
./tests/fdtester.sh cat < ./tests/lorem-ipsum
EOF
cat <<-EOF > $te 2>&1
0 -> ***
1 -> ***
2 -> ***
Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.
EOF
timeout 2s bash -c "./tesh < $ti" > $to 2>&1
displayNotTest $ti $to $te
test_nb=$((test_nb+1))
if ! grep -Pzo "0 ->.*\n1 ->.*\n2 ->.*\nLorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua." $to > /dev/null; then
  test_failed=$((test_failed+1))
  printf "${Red}==> Test#${test_nb} : ERROR outputs do not match.${Color_Off}\n"
else 
  test_passed=$((test_passed+1))
  printf "${Green}==> Test#${test_nb} : passed successfully.${Color_Off}\n"
fi

echo '######### Trying to test fd leaks 3'
cat <<-EOF > $ti 2>&1
./tests/fdtester.sh cat ./tests/lorem-ipsum > /tmp/TMPFILE
./tests/fdtester.sh cat ./tests/lorem-ipsum > /tmp/TMPFILE
./tests/fdtester.sh cat ./tests/lorem-ipsum >> /tmp/TMPFILE
echo sortie de echo
cat /tmp/TMPFILE
EOF
cat <<-EOF > $te 2>&1
sortie de echo
0 -> ***
1 -> ***
2 -> ***
Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.
0 -> ***
1 -> ***
2 -> ***
Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.
EOF
timeout 2s bash -c "./tesh < $ti" > $to 2>&1
displayNotTest $ti $to $te
test_nb=$((test_nb+1))
if ! grep -Pzo "0 ->.*\n1 ->.*\n2 ->.*\nLorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.\n0 ->.*\n1 ->.*\n2 ->.*\nLorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua." $to > /dev/null; then
  test_failed=$((test_failed+1))
  printf "${Red}==> Test#${test_nb} : ERROR outputs do not match.${Color_Off}\n"
else 
  test_passed=$((test_passed+1))
  printf "${Green}==> Test#${test_nb} : passed successfully.${Color_Off}\n"
fi

echo '######### Trying to test fd leaks 4'
cat <<-EOF > $ti 2>&1
cat ./tests/lorem-ipsum-long | ./tests/fdtester.sh grep Duis
EOF
cat <<-EOF > $te 2>&1
0 -> ***
1 -> ***
2 -> ***
Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur.
EOF
timeout 2s bash -c "./tesh < $ti" > $to 2>&1
displayNotTest $ti $to $te
test_nb=$((test_nb+1))
if ! grep -Pzo "0 ->.*\n1 ->.*\n2 ->.*\nDuis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur." $to > /dev/null; then
  test_failed=$((test_failed+1))
  printf "${Red}==> Test#${test_nb} : ERROR outputs do not match.${Color_Off}\n"
else 
  test_passed=$((test_passed+1))
  printf "${Green}==> Test#${test_nb} : passed successfully.${Color_Off}\n"
fi

fi


test_nb=$((test_nb+1))
if ldd tesh | grep -q libreadline; then
  echo "ERROR: 'tesh' is linked with libreadline"
  test_failed=$((test_failed+1))
  printf "${Red}==> Test#${test_nb} : ERROR outputs do not match.${Color_Off}\n"
else 
  echo "OK: 'tesh' is not linked with libreadline"
  test_passed=$((test_passed+1))
  printf "${Green}==> Test#${test_nb} : passed successfully.${Color_Off}\n"
fi


test_nb=$((test_nb+1))
if `ldd tesh | grep -q libdl`; then
  echo "OK: 'tesh' is linked with libdl"
  test_passed=$((test_passed+1))
  printf "${Green}==> Test#${test_nb} : passed successfully.${Color_Off}\n"
else 
  echo "ERROR: 'tesh' is not linked with libdl"
  test_failed=$((test_failed+1))
  printf "${Red}==> Test#${test_nb} : ERROR outputs do not match.${Color_Off}\n"
fi

echo -e "\n\n"


if [ $test_passed -eq $test_nb ] ; then
  printf "${Green}*** Test suite finished : all test cases ${test_passed}/${test_nb} passed ***${Color_Off}\n"
elif [ $test_passed -gt $test_failed ] ; then
  printf "${Green}*** Test suite finished : ${test_passed}/${test_nb} passed and ${Red}${test_failed}/${test_nb}${Green} failed ***${Color_Off}\n"
else
  printf "${Red}*** Test suite finished : ${Green}${test_passed}/${test_nb}${Red} passed and ${test_failed}/${test_nb} failed ***${Color_Off}\n"
fi