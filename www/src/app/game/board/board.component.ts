import { Component, OnInit } from '@angular/core';

import { Board } from './board';
import { Cell } from './cell';
import { Penguin } from './penguin';

declare var Module: any;

@Component({
  selector: 'app-board',
  templateUrl: './board.component.html',
  styleUrls: ['./board.component.scss']
})
export class BoardComponent implements OnInit {
  board: Board;

  nbHexagonal: number;
  nbPenguin: number;

  humanPlayerId: number = 1;

  // Wasm objects
  wasmGame: any;
  wasmBoard: any;
  wasmPenguins: any;

  constructor() { }

  ngOnInit(): void {
    this.board = new Board(this.nbHexagonal);
    this.board.generateMap();
  }

  ngOnDestroy(): void {
    console.log("Game destroyed");
    this.wasmGame.delete();
  }

  /***************************************************************************************************************************
  ************************************************ START GAME ****************************************************************
  ***************************************************************************************************************************/
  startWasmGame() {
    this.wasmGame = new Module.PenguinGame(this.nbHexagonal, this.nbPenguin);
    this.wasmBoard = this.wasmBoard.getBoard();
    this.board.generateMapFrom(this.wasmBoard);
    
    this.wasmPenguins = this.wasmBoard.getPlayersOnBoard();
  }

  /***************************************************************************************************************************
  ************************************************ PREVIEW *******************************************************************
  ***************************************************************************************************************************/
  // addHexagonal(): void {
  //   penguinGame.addHexagonal();
  // }

  // removeHexagonal(): void {
  //   penguinGame.removeHexagonal();
  // }

  // addPenguin(): void {
  //   penguinGame.addPenguin();
  // }

  // removePenguin(): void {
  //   penguinGame.removePenguin();
  // }
}
