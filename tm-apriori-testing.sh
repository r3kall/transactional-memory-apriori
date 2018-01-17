#!/usr/bin/env bash      

# ##################################################
# tm-apriori-testing.sh
# 
# run with 'bash tm-apriori-testing.sh filename max_threads min_support'
# 
# example 'bash tm-apriori-testing.sh database 8 0.0075'
#
# ##################################################


# Wrap function
function die () { local _message="${*} Exiting."; echo "$(_message)"; safeExit;}


# trapCleanup
# -----------------------------------
# Any actions that should be taken if the script is prematurely exited.
# -----------------------------------
function trapCleanup() {
	echo ""
	# Delete temp offsets
	if [ -d "${tmpDir}" ] ; then
		rm -r "${tmpDir}"
	fi
	die "Exit trapped. In function: '${FUNCNAME[*]}'."
}


# safeExit
# -----------------------------------
# Non destructive exit for when script exits naturally.
# -----------------------------------
function safeExit() {
	# Delete temp offsets
	if [ -d "${tmpDir}" ] ; then
		rm -r "${tmpDir}"
	fi
	trap - INT TERM EXIT
	exit
}


# Set Flags
# -----------------------------------
scriptName=$(basename "$0")
args=$@  # store arguments
N=5  # number of tests for each fixed number of threads.


# Set Temp Directory
# -----------------------------------
# Create temp directory with three random numbers and the process ID in the
# name to store the offsets. This directory is removed automatically at exit.
# -----------------------------------
tmpDir="./offsets.$RANDOM.$RANDOM.$RANDOM.$$"
(umask 077 && mkdir ${tmpDir}) || {
  die "Could not create temporary directory!"
}


# Check args
function check() {	

	# Check Number of Args
	if [ $# -ne 3 ]; then
		echo "Invalid number of arguments"
	    usage >&2; safeExit
	fi

	# check if the dataset exists
	if ! [ -e $1 ]; then
		echo "Invalid Input File"
		usage >&2; safeExit
	fi

	# check number of maximum threads
	if ! [[ $2 =~ ^[1-9]+$ ]]; then
		echo "Invalid Number of Threads"
		usage >&2; safeExit
	fi

	# check minimum support
	if ! [[ $3 =~ ^[0]+([.][0-9]+)$ ]]; then
		echo "Invalid Minimum Support"
		usage >&2; safeExit
	fi
}


# Make
function makeScript() {
	if ! [ -e "Makefile" ] ; then
		echo "Missing Makefile"
		usage >&2; safeExit
	fi

	make
	make offsets
}


# Main
function mainScript() {

	echo -n

	# check bash version
	echo "Bash version ${BASH_VERSION}..."

	# make offsets
	echo "Offset files production..."
	for (( c=1; c <= $2; c++ )) ; do
		./offsets $1 $c $tmpDir
	done

	# start tests
	echo "STM Apriori Testing...START"
	for (( c=1; c<=$2; c++ )) ; do
		for (( p=1; p<=$N; p++ )) ; do
			printf "n_thread: %d\n" $c
			./stm-apriori -i $1 -f $tmpDir/offset_P$c.txt -s $3 -n $c
		done
	done
	echo "STM Apriori Testing...END"
}


# Print usage
usage() {
	echo -n "
STM Apriori Testing Script.

${scriptName} [FILE] [MAX THREADS] [MIN SUPPORT]

Args:
	FILE:        valid database.
	MAX THREADS: (integer > 0) maximum number of used threads during the test.
	MIN SUPPORT: (0 < s < 1) support threshold.

"
}

# ############# ############# #############
# ##               						 ##
# ##             TIME TO RUN             ##
# ##                                     ##
# ############# ############# #############

# Trap bad exits with cleanup function.
trap trapCleanup EXIT INT TERM

# Exit on error.
set -o errexit

# Run functions
check $args
makeScript
mainScript $args
make clean

# Exit
safeExit
