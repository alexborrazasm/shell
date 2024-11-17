# TO DO

allocate -malloc Y
         -mmap Y
         -createshared
         -shared

deallocate -malloc Y
           -mmap Y
           -shared
           -delkey
           addr Y ADD mmap shared mode TODO

memfill addr 

memdump addr

memory -funcs Y
       -vars Y
       -blocks Y
       -all Y
       -pmap Y

readfile 

writefile

read

write

recurse Y