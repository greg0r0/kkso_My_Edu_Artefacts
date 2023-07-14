#compile classes
javac -Xlint:unchecked ./lab7_radix_sorts/*.java
#run main
java -ea ./main.java
#delete classes
rm -rf ./lab7_radix_sorts/*.class
