//go:generate fyne bundle -o data.go Icon.png

package main

import (
	"fyne.io/fyne/v2"
	"fyne.io/fyne/v2/app"
	"fyne.io/fyne/v2/container"
	"github.com/ratamahata/xgo/xfyneui"
)

type appInfo struct {
	name string
	icon fyne.Resource
	canv bool
	run  func(fyne.Window) fyne.CanvasObject
}

var appx = appInfo{"Tic Tac Toe", nil, true, xfyneui.Show}

func main() {

	a := app.New()
	a.SetIcon(resourceIconPng)

	content := container.NewMax()
	w := a.NewWindow("X & O")

	content.Objects = []fyne.CanvasObject{appx.run(w)}

	w.SetContent(content)
	w.Resize(fyne.NewSize(580, 605))
	w.ShowAndRun()
}
