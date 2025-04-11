CONCURRENT_LEVEL=5
NREQUEST=5000

make clean
make
./tiny 8000 2> out.log 1> /dev/null &
TINY_PID=$!
ab -n ${NREQUEST} -c ${CONCURRENT_LEVEL} localhost:8000/ &
apache_pid=$!
sleep 5
code out.log
jobs
echo ${TINY_PID}
kill -9 ${TINY_PID}
kill -9 ${apache_pid}
