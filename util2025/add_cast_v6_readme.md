# Aggiungere (int) a set_something, non sostituire:
awk -v func_name="set_something" -v target_cast_str="(int)" -v replace_existing=0 \
    -f add_cast_v6.awk infile > outfile

# Sostituire qualsiasi cast (...) esistente in set_something con (char*):
awk -v func_name="set_something" -v target_cast_str="(char*)" -v replace_existing=1 \
    -f add_cast_v6.awk infile > outfile

# Applicare a tutti i file .c (con GNU awk):
find . -type f -name '*.c' -exec awk -i inplace \
     -v func_name="set_something" -v target_cast_str="(char*)" -v replace_existing=1 \
     -f add_cast_v6.awk {} \;

# Esempio di sostituzione in tutti i file .c con cast esistente e backup del file 
find . -type f -name '*.c' -exec awk -i inplace -v inplace::suffix=.awk.bak -v func_name="set_something" -v target_cast_str="(XtArgVal)" -v replace_existing=1 -f util2025/add_cast_v6.awk {} \;

# eventuale cambiamento del nome set_something di tutti i file .c
find . -type f -name '*.c' -exec sed -i.sed.bak 's/set_something/set_something_val/g' {} \;
