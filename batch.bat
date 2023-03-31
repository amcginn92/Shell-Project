rm out.txt
touch out.txt
echo Beginning batch file processes. Thank you for joining us!
echo
echo Running 'io with args' from in.txt and then printing out.txt:
io with args > out.txt < in.txt
cat out.txt
echo
echo Running with redirection in reversed order:
rm out.txt
touch out.txt
io with args < in.txt > out.txt
cat out.txt
echo
echo Running with redirection for just input, should print in.txt to screen:
io with args < in.txt
echo
echo Running with redirection for just output, should output stdin to out.txt:
io with args > out.txt
exit
