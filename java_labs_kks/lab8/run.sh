#compile classes
javac -Xlint:unchecked ./lab8_linked_list_iterator/*.java
#run main
java -ea ./main.java
#delete classes
rm -rf ./lab8_linked_list_iterator/*.class
