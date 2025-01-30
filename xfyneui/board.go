package xfyneui

import (
	"fyne.io/fyne/v2"
	"fyne.io/fyne/v2/theme"
	"fyne.io/fyne/v2/widget"
	"time"
)

/*
#cgo CFLAGS: -m32
#cgo LDFLAGS: -m32 -Lxai -lGameBoard
#include "GameBoard.h"
*/
import "github.com/ratamahata/xgo/xai"

type board struct {
	gb       xai.GameBoard
	pieces   [15][15]int
	icons    [15][15]*boardIcon
	turn     int
	finished bool
}

func (b *board) result() uint8 {

	//// Check for a win in the diagonal direction from top left to bottom right.
	//if b.pieces[0][0] != 0 && b.pieces[0][0] == b.pieces[1][1] && b.pieces[1][1] == b.pieces[2][2] {
	//	return b.pieces[0][0]
	//}
	//
	//// Check for a win in the diagonal direction from bottom left to top right.
	//if b.pieces[0][2] != 0 && b.pieces[0][2] == b.pieces[1][1] && b.pieces[1][1] == b.pieces[2][0] {
	//	return b.pieces[0][2]
	//}
	//
	//for i := range b.pieces {
	//	// Check for a win in the horizontal direction.
	//	if b.pieces[i][0] != 0 && b.pieces[i][0] == b.pieces[i][1] && b.pieces[i][1] == b.pieces[i][2] {
	//		return b.pieces[i][0]
	//	}
	//
	//	// Check for a win in the vertical direction.
	//	if b.pieces[0][i] != 0 && b.pieces[0][i] == b.pieces[1][i] && b.pieces[1][i] == b.pieces[2][i] {
	//		return b.pieces[0][i]
	//	}
	//}

	return 0
}

func (b *board) newClick(row, column int) {
	//b.pieces[row][column] = b.turn%2 + 1

	xb := b.gb
	xb.GridClick(column, row)
	sync(b)

	//if b.turn > 3 {
	//	winner := b.result()
	//	if winner == 0 {
	//		if b.turn == 8 {
	//			dialog.ShowInformation("It is a tie!", "Nobody has won. Better luck next time.", fyne.CurrentApp().Driver().AllWindows()[0])
	//			b.finished = true
	//		}
	//		return
	//	}
	//
	//	number := string(winner + 48) // Number 1 is ascii #49 and 2 is ascii #50.
	//	dialog.ShowInformation("Player "+number+" has won!", "Congratulations to player "+number+" for winning.", fyne.CurrentApp().Driver().AllWindows()[0])
	//	b.finished = true
	//}

}

func (b *board) Reset() {

	b.finished = false
	b.turn = 1

	for r := 0; r < 15; r++ {
		for c := 0; c < 15; c++ {
			b.pieces[r][c] = 0
		}
	}

	go syncPeriodic(b)
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

	//if i.board.turn%2 == 0 {
	//	i.SetResource(theme.CancelIcon())
	//} else {
	//	i.SetResource(theme.RadioButtonIcon())
	//}

	i.board.newClick(i.row, i.column)
	i.board.turn++
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

func sync(b *board) {
	xb := b.gb

	for r := 0; r < 15; r++ {
		for c := 0; c < 15; c++ {

			code := xb.GetCell(r, c)

			if b.pieces[r][c] != code {
				b.pieces[r][c] = code

				if code > 0 {
					b.icons[r][c].SetResource(theme.CancelIcon())
				} else if code < 0 {
					b.icons[r][c].SetResource(theme.RadioButtonIcon())
				}
			}
		}
	}
}

func syncPeriodic(b *board) {
	for range time.Tick(time.Second * 2) {
		sync(b)
	}
}
