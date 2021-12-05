#!/bin/bash
# Made by Tom Gouville
set -e

echo '######### Checking AUTHORS file'
if grep -q 'NUSSBAUM Lucas' AUTHORS; then
  echo "ERROR AUTHORS file was not modified (not fatal for now)."
fi

echo '######### Checking rapport.pdf file'
if [ ! -f rapport.pdf ]; then
  echo "ERROR rapport.pdf does not exist (not fatal for now)."
fi

echo '######### Installing dependencies'
[[ -z $NODEP ]] && apt-get update
[[ -z $NODEP ]] && apt-get -y install --no-install-recommends build-essential libreadline-dev electric-fence python3 python3-pip
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
echo -e "\n\n"
mo=`md5sum < $to | awk '{print $1}'`
me=`md5sum < $te | awk '{print $1}'`
if ! [ "$mo" = "$me" ]; then
  echo "ERROR outputs do not match."
  exit 1
fi
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


if ldd tesh | grep -q libreadline; then
  echo "ERROR: 'tesh' is linked with libreadline"
  exit 1
else 
  echo "OK: 'tesh' is not linked with libreadline"
fi

if `ldd tesh | grep -q libdl`; then
  echo "OK: 'tesh' is linked with libdl"
else 
  echo "ERROR: 'tesh' is not linked with libdl"
  exit 1
fi