#kill $(lsof -t -i:8080)
killableproc=$(ps -ef | grep :8080)

kill -9 $(awk '{print $3}' killableproc)

echo "Prep logs----------------"
# remove old executable
rm a.out

# recompile
g++ servermain.cpp src/TCP_Server.cpp

# delete old logs
rm logs/*
# move last logs to archive
mv valgrind.log* logs/

# run with valgrind

echo "Program logs----------------"
G_SLICE=always-malloc G_DEBUG=gc-friendly valgrind --tool=memcheck --leak-check=full --track-origins=yes --num-callers=40 --log-file=valgrind.log /home/mark/Documents/c++/httpserv/a.out --log-file=valgrind.log /home/mark/Documents/c++/httpserv/a.out 
# if there were no leaks
if grep -q "All heap blocks were freed -- no leaks are possible" valgrind.log*; then
    res=$?
    echo "----------------"
    echo "there were NO leaks" $res
    echo "----------------"
else
    res=$?
    echo "----------------"
    echo "THERE WERE LEAKS" $res
    echo "----------------"
fi
