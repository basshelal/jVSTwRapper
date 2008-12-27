/* 
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER
 * Copyright 2008 Sun Microsystems, Inc. All rights reserved. Use is subject to license terms. 
 * 
 * This file is available and licensed under the following license:
 * 
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice, 
 *     this list of conditions and the following disclaimer.
 *
 *   * Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *
 *   * Neither the name of Sun Microsystems nor the names of its contributors 
 *     may be used to endorse or promote products derived from this software 
 *     without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
package jvst.examples.javafx.fxdemo.displayshelf;

import javafx.animation.*;
import javafx.stage.*;
import javafx.scene.*;
import javafx.scene.shape.*;
import javafx.scene.paint.*;
import javafx.scene.effect.*;
import javafx.scene.image.*;
import javafx.scene.input.*;
import javafx.scene.text.*;

import jvst.examples.javafx.fxdemo.JavaInterop;


public class Main extends JavaInterop {

    var images = [
        "DSC_0026_2.jpg",
        "DSC_0040_2.jpg",
        "DSC_0068_2.jpg",
        "DSC_0083_2.jpg",
        "DSC_0094_2.jpg",
        "DSC_0129_2.jpg",
        "DSC_0152_2.jpg",
        "DSC_0162_2.jpg",
        "DSC_0172_2.jpg"
        ];

    var half = images.size()/2;
    var shelf:DisplayShelf;
    var width = 600;
    var scene:Scene;

    init {
        shelf = DisplayShelf {
            spacing: 50
            scaleSmall: 0.7
            leftOffset: -110
            rightOffset: 110
            perspective: true
            content: bind for(i in images) {
                var item:Item = Item {
                    angle: 45
                    position: indexof i - half
                    image:Image { url: "{__DIR__}photos/{i}" }
                    shelf: bind shelf
                };
                item;
            }

            onKeyPressed:function(e:KeyEvent):Void {
                if(e.code == KeyCode.VK_LEFT) {
                    shelf.shift(1);
                }
                if(e.code == KeyCode.VK_RIGHT) {
                    shelf.shift(-1);
                }
            }
        }

        shelf.translateX = width/2 - 200/2;
        shelf.translateY = 50;

        scene = Scene {
            content: [
                Rectangle {
                    width: width
                    height: 300
                    fill: LinearGradient {
                        startX: 0 startY: 0
                        endX: 0 endY: 1
                        proportional: true
                        stops: [
                            Stop { offset: 0.0 color: Color.rgb(150,150,150) }
                            Stop { offset: 1.0 color: Color.rgb(30,30,30)},
                        ]
                    }
                },
                shelf
            ]
        }
    }
    

    public override function getScene() : Object {
        return scene;
    }

}