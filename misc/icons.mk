all: rott.xpm ../WinRT-8.1/Rott94/Rott94.Windows/Assets/Logo.png ../WinRT-8.1/Rott94/Rott94.Windows/Assets/SmallLogo.png ../WinRT-8.1/Rott94/Rott94.Windows/Assets/SplashScreen.png ../WinRT-8.1/Rott94/Rott94.Windows/Assets/StoreLogo.png ../WinRT-8.1/Rott94/Rott94.WindowsPhone/Assets/Logo.scale-240.png ../WinRT-8.1/Rott94/Rott94.WindowsPhone/Assets/SmallLogo.scale-240.png ../WinRT-8.1/Rott94/Rott94.WindowsPhone/Assets/SplashScreen.scale-240.png ../WinRT-8.1/Rott94/Rott94.WindowsPhone/Assets/Square71x71Logo.scale-240.png ../WinRT-8.1/Rott94/Rott94.WindowsPhone/Assets/StoreLogo.scale-240.png ../WinRT-8.1/Rott94/Rott94.WindowsPhone/Assets/WideLogo.scale-240.png ../android/res/drawable-hdpi/ic_launcher.png ../androidres/drawable-mdpi/ic_launcher.png ../android/res/drawable-xhdpi/ic_launcher.png ../android/res/drawable-xxhdpi/ic_launcher.png

.PHONY: all

rott.xpm: rott.png
	convert $^ $@

rott.png: rott.svg
	inkscape --export-png=$@ $^

../WinRT-8.1/Rott94/Rott94.Windows/Assets/Logo.png:  rott.svg
	inkscape -D -w 150 -h 150 --export-png=$@ $^

../WinRT-8.1/Rott94/Rott94.Windows/Assets/SmallLogo.png:  rott.svg
	inkscape -D -w 30 -h 30 --export-png=$@ $^

../WinRT-8.1/Rott94/Rott94.Windows/Assets/SplashScreen.png: rott.svg
	inkscape -D -w 620 -h 300 --export-png=$@ $^

../WinRT-8.1/Rott94/Rott94.Windows/Assets/StoreLogo.png: rott.svg
	inkscape -D -w 50 -h 50 --export-png=$@ $^

../WinRT-8.1/Rott94/Rott94.WindowsPhone/Assets/Logo.scale-240.png: rott.svg
	inkscape -D -w 360 -h 360 --export-png=$@ $^

../WinRT-8.1/Rott94/Rott94.WindowsPhone/Assets/SmallLogo.scale-240.png: rott.svg
	inkscape -D -w 106 -h 106 --export-png=$@ $^

../WinRT-8.1/Rott94/Rott94.WindowsPhone/Assets/SplashScreen.scale-240.png: rott.svg
	inkscape -D -w 1152 -h 1920 --export-png=$@ $^

../WinRT-8.1/Rott94/Rott94.WindowsPhone/Assets/Square71x71Logo.scale-240.png: rott.svg
	inkscape -D -w 170 -h 170 --export-png=$@ $^

../WinRT-8.1/Rott94/Rott94.WindowsPhone/Assets/StoreLogo.scale-240.png: rott.svg
	inkscape -D -w 120 -h 120 --export-png=$@ $^

../WinRT-8.1/Rott94/Rott94.WindowsPhone/Assets/WideLogo.scale-240.png: rott.svg
	inkscape -D -w 744 -h 360 --export-png=$@ $^

../android/res/drawable-hdpi/ic_launcher.png: rott.svg
	inkscape -D -w 72 -h 72 --export-png=$@ $^

../androidres/drawable-mdpi/ic_launcher.png: rott.svg
	inkscape -D -w 48 -h 48 --export-png=$@ $^

../android/res/drawable-xhdpi/ic_launcher.png: rott.svg
	inkscape -D -w 96 -h 96 --export-png=$@ $^

../android/res/drawable-xxhdpi/ic_launcher.png: rott.svg
	inkscape -D -w 144 -h 144 --export-png=$@ $^
