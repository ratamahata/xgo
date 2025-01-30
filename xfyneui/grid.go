package xfyneui

import (
	"fyne.io/fyne/v2"
	"fyne.io/fyne/v2/container"
	"fyne.io/fyne/v2/theme"
	"fyne.io/fyne/v2/widget"
	"github.com/ratamahata/xgo/xai"
)

// Show loads a tic-tac-toe example window for the specified app context
func Show(win fyne.Window) fyne.CanvasObject {

	gb := xai.GetXBoard(0)
	board := &board{gb: gb}

	grid := container.NewGridWithColumns(15)
	for r := 0; r < 15; r++ {
		for c := 0; c < 15; c++ {
			grid.Add(newBoardIcon(r, c, board))
		}
	}

	reset := widget.NewButtonWithIcon("Reset Board", theme.ViewRefreshIcon(), func() {
		for i := range grid.Objects {
			grid.Objects[i].(*boardIcon).Reset()
		}

		board.Reset()
	})

	board.Reset()

	return container.NewBorder(reset, nil, nil, nil, grid)
}
