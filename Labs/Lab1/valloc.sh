echo "Verifying"
if [ -d "\$VTOP" ]
then
	echo "\$VTOP exists already, exiting"
	exit
else
	mkdir "\$VTOP"
	mkdir "\$VTOP/etc"
	mkdir "\$VTOP/src"
	echo "Created \$VTOP and subdirs"
	exit
fi
