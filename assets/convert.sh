# Uses imagemagick convert tool to convert png's
# into a format that our game-engine can currently
# suppport

# To install imagemagick tool suite
# (which has the 'convert' tool used here) on ubuntu:
# 'sudo apt update'
# 'sudo apt install imagemagick'

srcfolder=original # the folder name where we read the unconverted png's from
cd images

count=0

for filename in $srcfolder/*; do
	bname=$( basename "$filename")
	finalname=$( basename -s .png "$filename").cng
	if ! test -f $finalname; then
		echo "	$bname -> CNG"
		convert $filename \
			-compress None \
			-type palette \
			-define png:compression-level=0 \
			-define png:color-type=3 \
			-define png:bit-depth=8 \
			-define PNG:exclude-chunk=chrm,gAMA,bKGD,pHYs,tIME \
			-quality 100 \
			-density 600 \
			-colors 255 \
			-interlace none \
			-channel RGB \
		$finalname
		((count=count+1))
	fi
done

echo "Converted $count images to cyberpng."

#		-colors 1 \