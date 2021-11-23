import { Injectable } from '@angular/core';
import { Subject } from 'rxjs';

@Injectable({
  providedIn: 'root'
})
export class UtilService {

  private modal = new Subject<Boolean>();
  public modalEmitter = this.modal.asObservable();

  constructor() { }


  isModalEmitChange(show:boolean) {
    this.modal.next(show);
  }
}
