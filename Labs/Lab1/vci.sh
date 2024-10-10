for x in "$@"
do
	echo trying "$x"
	if [ -d "\$VTOP/src/$x" ]
	then
		echo "$x" exists, exiting
		continue
	else
		mkdir "\$VTOP/src/$x"
	fi	
done
