#compile classes
javac -Xlint:unchecked ./lab6_fast_sorting/*.java
#run main
java -ea ./main.java
#delete classes
rm -rf ./lab6_fast_sorting/*.class
