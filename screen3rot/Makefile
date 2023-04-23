rec:
	openmsx -machine Panasonic_FS-A1GT -ext gfx9000 -ext Moonsound -diska emul/dsk/DOS1_template.dsk -command "set speed 9999;after time 13.5 \"set speed 100;record start aaa\""
mp4:
	ffmpeg -y -i ~/.openMSX/videos/aaa.avi -b 1500k -r 30  -vf "scale=960:720" -vcodec libx264 -pix_fmt yuv420p aaa.mp4
gif:
	mkdir -p mov
	ffmpeg -y -i ~/.openMSX/videos/aaa.avi -r 30 mov/%04d.png
	convert +map  -delay 3 mov/*.png aaa.gif
	rm -rf mov
	code aaa.gif
