: string-concat dup
count
rot
dup
count
rot
over
+
heap-alloc
rot
swap
dup
rot
string-copy
dup
rot
+
rot
string-copy ;
