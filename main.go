//go:generate fyne bundle -o data.go Icon.png

package main

import (
	"fyne.io/fyne/v2"
	"fyne.io/fyne/v2/app"
	"fyne.io/fyne/v2/container"
	"github.com/ratamahata/xgo/xfyneui"
)

/*
#cgo CFLAGS: -m32
#cgo LDFLAGS: -m32 -Lxai -lGameBoard
#include "GameBoard.h"
*/
import "github.com/ratamahata/xgo/xai"

type appInfo struct {
	name string
	icon fyne.Resource
	canv bool
	run  func(fyne.Window) fyne.CanvasObject
}

var appx = appInfo{"Tic Tac Toe", nil, true, xfyneui.Show}

func main() {

	xb := xai.CreateXBoard(0)

	//TODO the line below is just to ensure we can run the C++ code from here
	//its need to remove it and instantiate GameBoard instead, and also connect it to the UI

	//xb.Put('0')
	xb.Build()
	//x := xai.NewGameBoard()

	a := app.New()
	a.SetIcon(resourceIconPng)

	content := container.NewMax()
	w := a.NewWindow("X & O")

	content.Objects = []fyne.CanvasObject{appx.run(w)}

	w.SetContent(content)
	w.Resize(fyne.NewSize(480, 360))
	w.ShowAndRun()
}
