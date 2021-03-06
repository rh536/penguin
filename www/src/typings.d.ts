/*
 * Extra typings definitions
 */

// Allow .json files imports
declare module '*.json';

// SystemJS module definition
declare var module: NodeModule;
interface NodeModule {
  id: string;
}

declare module 'Module';

declare module 'animejs/lib/anime.es';
declare module 'number-flip';
