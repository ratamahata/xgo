package xfyneui

import (
	"fyne.io/fyne/v2"
	"fyne.io/fyne/v2/dialog"
	"fyne.io/fyne/v2/theme"
	"fyne.io/fyne/v2/widget"
	"time"
)

import "github.com/ratamahata/xgo/xai"

type board struct {
	gb                  xai.Grower
	pieces              [15][15]int
	icons               [15][15]*boardIcon
	displayedMovesCount int
	finished            bool
	lastMove            *boardIcon
	lMcoolDown          int
}

func (b *board) newClick(row, column int) {

	xb := b.gb
	xb.GridClick(column, row)
	sync(b, true)

	if b.displayedMovesCount >= 9 {

		hasWinner := xb.GetRResult() >= 32600 //|| xb.ResultRecieved <= -32600

		if hasWinner {
			number := string((b.displayedMovesCount % 2) + 48) // Number 1 is ascii #49 and 2 is ascii #50.
			dialog.ShowInformation("Player "+number+" has won!", "Congratulations to player "+number+" for winning.", fyne.CurrentApp().Driver().AllWindows()[0])
			b.finished = true
		}

		if b.displayedMovesCount == 225 {
			dialog.ShowInformation("It is a tie!", "Nobody has won. Better luck next time.", fyne.CurrentApp().Driver().AllWindows()[0])
			b.finished = true
		}
		return
	}
}

func (b *board) Reset() {

	b.gb.RestartClick()
	b.finished = false
	b.displayedMovesCount = 0

	for r := 0; r < 15; r++ {
		for c := 0; c < 15; c++ {
			b.pieces[r][c] = 0
		}
	}

	b.lastMove = nil
	b.lMcoolDown = 0
}

type boardIcon struct {
	widget.Icon
	board       *board
	row, column int
}

func (i *boardIcon) Tapped(ev *fyne.PointEvent) {
	if i.board.pieces[i.row][i.column] != 0 || i.board.finished {
		return
	}

	i.board.newClick(i.row, i.column)
}

func (i *boardIcon) Reset() {
	i.SetResource(theme.ViewFullScreenIcon())

}

func newBoardIcon(row, column int, board *board) *boardIcon {
	i := &boardIcon{board: board, row: row, column: column}
	i.SetResource(theme.ViewFullScreenIcon())
	i.ExtendBaseWidget(i)
	board.icons[row][column] = i
	return i
}

func sync(b *board, isClick bool) {

	xb := b.gb

	actualMoves := xb.GetMovesCount()
	defaultSize := b.icons[0][0].Size()

	if b.lastMove != nil && actualMoves >= 7 {

		if b.lastMove.Size().Width < defaultSize.Width {
			b.lastMove.Resize(b.lastMove.Size().AddWidthHeight(defaultSize.Width/10.0, defaultSize.Height/10))
		} else {
			if b.lMcoolDown > 0 {
				b.lMcoolDown--
			} else {
				b.lastMove.Resize(b.lastMove.Size().SubtractWidthHeight(defaultSize.Width/10.0, defaultSize.Height/10))
				b.lMcoolDown = 12
			}
		}
	}

	if !isClick && actualMoves == b.displayedMovesCount {
		return
	}

	b.displayedMovesCount = actualMoves

	for r := 0; r < 15; r++ {
		for c := 0; c < 15; c++ {

			code := xb.GetCell(r, c)

			if b.pieces[r][c] != code {
				b.pieces[r][c] = code

				if code > 0 {

					lastMove := b.icons[r][c]
					lastMove.SetResource(theme.CancelIcon())

					if b.lastMove != lastMove {

						if b.lastMove != nil {
							b.lastMove.Resize(defaultSize)
						}
						b.lastMove = lastMove

						if actualMoves >= 7 {
							b.lastMove.Resize(b.lastMove.Size().SubtractWidthHeight(defaultSize.Width*0.2, defaultSize.Height*0.2))
						}
					}
				} else if code < 0 {
					b.icons[r][c].SetResource(theme.RadioButtonIcon())
				}
			}
		}
	}
}

func syncPeriodic(b *board) {
	for range time.Tick(time.Millisecond * 400) {
		sync(b, false)
	}
}
