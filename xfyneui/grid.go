package xfyneui

import (
	"fyne.io/fyne/v2"
	"fyne.io/fyne/v2/container"
	"fyne.io/fyne/v2/theme"
	"fyne.io/fyne/v2/widget"
	"github.com/ratamahata/xgo/xai"
)

// Show loads a tic-tac-toe example window for the specified app context
func Show(win fyne.Window, sd *StatusController) fyne.CanvasObject {
	gb := xai.GetXBoard(0)
	go gb.Grow()

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

		board.Reset(false)
	})

	board.Reset(true)

	// PASS the status controller to your periodic function
	go syncPeriodic(board, sd)

	return container.NewBorder(reset, nil, nil, nil, grid)
}
