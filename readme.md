# TO DO

allocate -malloc Y
         -mmap 
         -createshared
         -shared

deallocate -malloc Y
           -mmap
           -shared
           -delkey
           addr Y

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