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
