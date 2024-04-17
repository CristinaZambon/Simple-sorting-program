//MARIA CRISTINA ZAMBON mz348
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>
#include "sort.h"
#include <sys/types.h>
#include <sys/stat.h>
#define RECORD_SIZE 100

void usage(char *prog) 
{
    fprintf(stderr, "usage: %s -i inputfile -o outputfile\n", prog);
    exit(1);
}
int compare (const void * a, const void * b);

int main(int argc, char *argv[])
{
    int c; //char to check arguments on command line
    char *inFile   = "/no/such/file"; //name of input file
    char *outFile   = "/no/such/file";  //name of output file
    
    //check the arguments and the flags on command line. If wrong, print error message.
    while ((c = getopt(argc, argv, "i:o:")) != -1) {
	switch (c) {
	case 'i':
	    inFile =  strdup(optarg); //convert the argument into a string and copy it
	    break;
	case 'o':
	    outFile = strdup(optarg);
	    break;
	default:
	    usage(argv[0]); //print error message
	}
    }
    
//open the input file and check if there's an error.    
    int fdInput = open(inFile, O_RDONLY);
    if (fdInput < 0) {
    fprintf(stderr, "Error: Cannot open file %s\n", inFile);
	exit(1);
    }
    
    
//compute the number of records = lines in the inputfile
// st_size returns the total number of bytes in the input file
//( total number of bytes in the file) / (byte-size of a record) = total number of records in the file
    struct stat inputfileStat;
     fstat(fdInput, &inputfileStat);
     int n_records = (int) inputfileStat.st_size/RECORD_SIZE;
printf("\n%d is the n records\n", n_records);


//define a dynamic array of structure rec_t to store the content of the file
    rec_t * allRecords;
    allRecords = (rec_t *)malloc(n_records * sizeof(rec_t));
    
    if (allRecords==NULL){
        fprintf(stderr, "Memory allocation error \n");
	    exit(1);
    }

//read each record from input file and store it in the dynamic array
    int i=0;
    while (1) {	
	int rc = read(fdInput, &allRecords[i], sizeof(rec_t));
	if (rc == 0) // 0 indicates EOF
	    break;
	if (rc < 0) {
    fprintf(stderr, "Error: cannot read correctly file %s\n", inFile);
	    exit(1);
	}
	++i;}
//close the inputfile
if ( close(fdInput)!=0 ){
     fprintf(stderr, "Error closing file %s\n", inFile);
	    exit(1);
}


//sort the dynamic array according to the key
    
     for(i=0;i<n_records;++i){
    printf("\n%d", allRecords[i].key);
    }
    qsort(allRecords, n_records, sizeof(rec_t), compare);
    
    printf("\nAfter sorting \n");
    for(i=0;i<n_records;++i){
    printf("\n%d", allRecords[i].key);
    }
 
   
//write the new sorted list of records on the output file
//check if errors
int fd = open(outFile, O_WRONLY|O_CREAT|O_TRUNC, S_IRWXU);
    if (fd < 0) {
    fprintf(stderr, "Error: Cannot open file %s\n", outFile);
	exit(1);
    }

for(i=0;i<n_records;++i){
    int rc = write(fd, &allRecords[i], sizeof(rec_t));
	if (rc != sizeof(rec_t)) {
	   fprintf(stderr, "Error: Cannot write on file %s\n", outFile);
	    exit(1);}
}


//close the output file
if ( close(fd)!=0 ){
     fprintf(stderr, "Error closing file %s\n", outFile);
	    exit(1);
}

free(allRecords); // free the memory allocated dynamically
}  

int compare (const void *av, const void *bv)
{      const  rec_t *a = av, *b = bv;

        if (a->key < b->key) return -1;
        if (a->key > b->key) return 1;
        return 0;
}




