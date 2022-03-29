COMPILAZIONE ED ESECUZIONE

g++ -o main *.cpp
./main

----------------------------------------

Non e' necessario nessun argomento aggiuntivo da riga di comando
il programma chiede N e prende in automatico il file dalla cartella "input" con il nome corretto e scrive nella cartella "output"

./input/
	inputaudio1.data
	inputaudio2.data
	.
	.
	inputaudioN.data

./output/
	outputVAD1.data
	outputVAD1.txt
	.
	.
	outputVADN.data
	outputVADN.txt

----------------------------------------

variabili modificabili dal file Vad.hpp
in particolare NEXT_PACKET e' il contatore spiegato in FlowChart.pdf per decidere l numero di pacchetti da salvare dopo l'ultiva voce trovata

PI = 3.14159265358979323846
MAX_FREQ = 3400
MIN_FREQ = 200
PACKET_SIZE = 160
NEXT_PACKET = 2;                     
INPUT_FOLDER = "input/"
OUTPUT_FOLDER = "output/"
FILE_EXTENSION = ".data"

