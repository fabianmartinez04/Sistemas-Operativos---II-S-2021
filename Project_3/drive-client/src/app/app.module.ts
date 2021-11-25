import { NgModule } from '@angular/core';
import { BrowserModule } from '@angular/platform-browser';

import { AppRoutingModule } from './app-routing.module';
import { AppComponent } from './app.component';
import { FormsModule } from '@angular/forms';
import { FileComponent } from './components/file/file.component';
import { UserComponent } from './components/user/user.component';
import { DashboardComponent } from './components/dashboard/dashboard.component';
import { CreateDriveComponent } from './components/create-drive/create-drive.component';
import { MoveComponent } from './components/move/move.component';
import { SharedComponent } from './components/shared/shared.component';
import { CopyComponent } from './components/copy/copy.component';
import { CopyRvComponent } from './components/copy-rv/copy-rv.component';
import { CopyVrComponent } from './components/copy-vr/copy-vr.component';
import { CreateFileComponent } from './components/create-file/create-file.component';
import { CreateFolderComponent } from './components/create-folder/create-folder.component';

@NgModule({
  declarations: [
    AppComponent,
    FileComponent,
    UserComponent,
    DashboardComponent,
    CreateDriveComponent,
    MoveComponent,
    SharedComponent,
    CopyComponent,
    CopyRvComponent,
    CopyVrComponent,
    CreateFileComponent,
    CreateFolderComponent 
  ],
  imports: [
    BrowserModule,
    AppRoutingModule,
    FormsModule
  ],
  providers: [],
  bootstrap: [AppComponent]
})
export class AppModule { }
