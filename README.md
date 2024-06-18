# A Privacy-Preserving Approach to Conformance Checking


### Install dependencies
To install required libraries (protobuf, nng, poco) run the command:

```
make install_deps
```


### Build the project:

```
mkdir build
cd build
cmake ..
make
```


### Input File Format:

The input file format for the software is a text file containing a sequence (the set of traces separated by `;` and termiated with `$`). 
For example:

```
  ABD;ABCBD;$
```

The client query file format is:

```
  <query_size>
  <01-Rank size>
  <query>
```

For example:

```
4
12
5 3 4 2
```

### Running Example

To run the 01_simple example through cmake the "tools/resources/simple_01.txt" and ¨client/resources/simple_01_query.txt¨ files will be used (you can modify the line 11 on the CMakeLists.tx file to change the filenames to be used).

```
set(input_file "simple_01")  #set the name without .txt extension
```

The "simple_01.txt" file on the ¨tools/resources/¨ path will be used.
To generate the integer maping and the integer sequence it is required to run the command (you need to build the project first):

```
  cd build
  make run_chtoi
```

This command generate the file simple_01_to_ints.txt.
To compute the Burrows-Wheeler transform you can run the commands:

```
  cd build
  make run_bwt
```
this command will generate the file simple_01_to_ints_bwt.txt.
To compute the 01-Rank matrix you can run the commands:

```
  cd build
  make run_WM
```
this command will generate the file simple_01_to_ints_bwt_wm.txt (this file will be used by the server).

#### Starting the Server

To start the server through cmake:

```
  cd build
  make runs_f
```
This command start the server and load the 01-Rank Matrix.


#### Starting the Client

To start the clieng through cmake (server must be running, it is recommended open a new window terminal):

```
cd build
make runc_f
```
This command will start the client, the "client/resources/simple_01_query.txt" file will be used to generate the query (check the query file format).

At the end of the client execution the software will report the sequence found:

```
Query found: 5-<3>-4-1-
```

This example tells that the 3 value was a log move.


### Executables.

The build step generates a set of executables:

Tools: 

  - __chtoi__: Converts a text sequence to integer representation. Create the alphabet used on next steps.
    - ```
      ./chtoi [input_file]
      ``` 
  - __btw__: Generate the Burrows-Wheleer Transform.
    -  ```
       ./bwt < [input_filename] > [output_filename]
       ```       
  - __makeWM__: Compute the Wavelete-Matrix and the 01-Rank Matrix.
    - ```
      ./makeWM < [input_filename] > [output_filename]
      ```

Client-Server

  - __server_app__: Server applications.
    - ```
        ./server_app -i <input_filename> -d <allowed_deletes> -p <port>
      ```
      The <input_filename> is the filename that contains the 01-Rank matrix.
  - __client_app__: Client application
    - ```
        ./client_app: -i <input_filename> -s <server_name/IP address> -p <Server_por>
      ```
      The <input_filename> is the filename that contains the client query.


 These executables are used and configured on CMake. You can use it separately.     
