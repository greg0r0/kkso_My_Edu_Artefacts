#compile classes
javac -Xlint:unchecked ./lab5_base_sorting/*.java
#run main
java -ea ./main.java
#delete classes
rm -rf ./lab5_base_sorting/*.class
