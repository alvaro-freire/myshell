# Runs main program with Valgrind
valgrind --leak-check=full --show-leak-kinds=all --leak-resolution=med --track-origins=yes --verbose --vgdb=no ./shell3
