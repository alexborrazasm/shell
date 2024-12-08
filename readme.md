# TO DO

getuid [X]

setuid [-l] id [X]

showvar v1 v2 .. [X]

changevar [-a|-e|-p] var val [x] QUEDA MEMORIA?

subsvar [-a|-e] v1 v2 val [X] TB QUEDA MEMORIA

environ [-environ|-addr] [X]

fork [X]

search [X]
       -add dir [X]
       -del dir [X]
       -clear [X]
       -path [X]

exec progspec [X] queda memoria, PERO EL PROCESO REMPLAZA MI SHELL? no se

execpri prio progspec [X] revisar prio y queda memoría mismo arriba

fg progspec [X]

fgpri progspec [X]

back progspec [] queda men, falta añadir a lista

backpri prio progspec [] queda men, falta añadir a lista

listjobs []

deljobs -term|-sig []