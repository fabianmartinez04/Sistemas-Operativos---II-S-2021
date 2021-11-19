import { ComponentFixture, TestBed } from '@angular/core/testing';

import { CopyVrComponent } from './copy-vr.component';

describe('CopyVrComponent', () => {
  let component: CopyVrComponent;
  let fixture: ComponentFixture<CopyVrComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [ CopyVrComponent ]
    })
    .compileComponents();
  });

  beforeEach(() => {
    fixture = TestBed.createComponent(CopyVrComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
