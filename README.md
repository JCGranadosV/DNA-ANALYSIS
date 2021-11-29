# DNA-ANALYSIS
C Program that analyzes DNA samples and compares them line by line to a Large genome, executed as a thread, analyzing the sequence in parallel.

The program has a client who interacts with the server that has 2 main functions:
- Upload_Reference:
Uploads the main DNA Genome to which we will compare our DNA samples and stores it in main memory.
- Upload_Sequence:
Uploads line by line of the DNA sample and compares it to the Genome, it outputs if the line was included, and if it was, in which position it was found.
At the end of the iteration, the server calculates the percentage of sequence covered by the samples to get a match centage.
Finally, it displays the mapped and non-mapped sequences to the user.

Program developed by:
Juan Camilo Granados, 
Alberto Jorge Treviño, 
Jorge Abdo
